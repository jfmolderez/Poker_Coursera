#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int card_ptr_comp(const void * vp1, const void * vp2) {
  const card_t * const * cp1 = vp1;
  const card_t * const * cp2 = vp2;
  unsigned v1 = (*cp1)->value;
  unsigned v2 = (*cp2)->value;
  suit_t s1 = (*cp1)->suit;
  suit_t s2 = (*cp2)->suit;

  if (v1 != v2) {
    return (v2 - v1);
  }
  if (s1 != s2) {
    return (s2 - s1);
  }

  return 0;
}

suit_t flush_suit(deck_t * hand) {
  card_t ** ptr = hand->cards;
  size_t n = hand->n_cards;
  int n_clubs = 0;
  int n_diamonds = 0;
  int n_hearts = 0;
  int n_spades = 0;

  for (size_t i = 0; i < n; i++) {
    card_t * ptrc = *(ptr + i);
    switch(ptrc->suit) {
    case CLUBS:
      n_clubs++;
      if (n_clubs >= 5) {
        return CLUBS;
      }
      break;
    case DIAMONDS:
      n_diamonds++;
      if (n_diamonds >= 5) {
        return DIAMONDS;
      }
      break;
    case HEARTS:
      n_hearts++;
      if (n_hearts >= 5) {
        return HEARTS;
      }
      break;
    case SPADES:
      n_spades++;
      if (n_spades >= 5) {
        return SPADES;
      }
      break;
    case NUM_SUITS:
      break;
    }
  }
  return NUM_SUITS;
}

unsigned get_largest_element(unsigned * arr, size_t n) {
  unsigned max = 0;
  unsigned current;
  for(size_t i = 0; i < n; i++) {
    current = *(arr + i);
    if (current > max) {
      max = current;
    }
  }
  return max;
}

size_t get_match_index(unsigned * match_counts, size_t n, unsigned n_of_akind){

  unsigned * ptr = match_counts;
  for(size_t i = 0; i < n; i++) {
    if (*ptr == n_of_akind) {
      return i;
    }
    ptr++;
  }
  // should never arrive here
  return 999;
}

ssize_t find_secondary_pair(deck_t * hand,
                             unsigned * match_counts,
                             size_t match_idx) {
  card_t ** ptrh = hand->cards;
  size_t n = hand->n_cards;
  unsigned * ptrm = match_counts;

  for(ssize_t i = 0; i < n; i++) {
    if (*ptrm > 1) {
      if ((*(ptrh + i))->value != (*(ptrh + match_idx))->value) {
        return i;
      }
    }
    ptrm++;
  }
  return -1;
}

int is_n_length_straight_at(deck_t * hand, size_t index, suit_t fs, int n) {

  card_t ** ptrh = hand->cards;
  size_t nc = hand->n_cards;
  unsigned val = (*(ptrh + index))->value;
  suit_t s = (*(ptrh + index))->suit;

  if (fs != NUM_SUITS && s != fs) { // base case : straight flush
    return 0;
  }

  if (n == 2) { // base case : n == 2
    // find the the last in the straight
    if (fs != NUM_SUITS) {
      for (size_t i = index + 1; i < nc; i++) {
        if (((*(ptrh + i))->value == val - 1) && ((*(ptrh + i))->suit == s)){
          return 1;
        }
      }
      return 0;
    }
    // fs == NUM_SUITS
    for(size_t i = index + 1; i < nc; i++) {
      if ((*(ptrh + i))->value == val - 1) {
          return 1;
      }
    }
    return 0;
  }

  // recursive case n > 2
  for(size_t i = index + 1; i < nc; i++) {
    if (((*(ptrh + i))->value == val - 1) && is_n_length_straight_at(hand, i, fs, n-1)) {
        return 1;
    }
  }
  return 0;
}

int is_ace_low_straight_at(deck_t * hand, size_t index, suit_t fs){

  card_t ** ptrh = hand->cards;
  size_t nc = hand->n_cards;

  if (fs != NUM_SUITS && (*(ptrh + index))->suit != fs) {
    return 0;
  }
  for (size_t i = index + 1; i < nc-3; i++) {
    if (((*(ptrh + i))->value == 5) && is_n_length_straight_at(hand, i, fs, 4)){
      return 1;
    }
  }
  return 0;
}

int is_straight_at(deck_t * hand, size_t index, suit_t fs) {

  card_t ** ptrh = hand->cards;

  if ((*(ptrh + index))->value == VALUE_ACE) {
    if(is_ace_low_straight_at(hand, index, fs)){
      return -1;
    }
  }
  // no low ace straight
  return is_n_length_straight_at(hand, index, fs, 5);
}

hand_eval_t build_hand_from_match(deck_t * hand,
                                  unsigned n,
                                  hand_ranking_t what,
                                  size_t idx) {

  hand_eval_t ans;
  card_t ** ptrh = hand->cards;

  ans.ranking = what;
  for (size_t i = 0; i < n; i++){
    ans.cards[i] = *(ptrh + idx + i);
  }
  if (idx == 0) {
    for(size_t i = 0; i < 5-n; i++){
      ans.cards[n+i] = *(ptrh + n + i);
    }
  } else {
    if (idx >= 5-n) {
      for(size_t i = 0; i < 5-n; i++){
        ans.cards[n+i] = *(ptrh + i);
      }
    } else {
      for(size_t i = 0; i < idx; i++){
        ans.cards[n+i] = *(ptrh + i);
      }
      for(size_t i = 0; i < 5-n-idx; i++){
        ans.cards[n+idx+i] = *(ptrh + idx + n + i);
      }
    }
  }
  return ans;
}

int compare_hands(deck_t * hand1, deck_t * hand2) {
  // sort each hand
  qsort(hand1->cards, hand1->n_cards, sizeof *(hand1->cards), card_ptr_comp);
  qsort(hand2->cards, hand2->n_cards, sizeof *(hand2->cards), card_ptr_comp);
  // evaluate the hands
  hand_eval_t he1 = evaluate_hand(hand1);
  hand_eval_t he2 = evaluate_hand(hand2);
  // Check the rankings
  if (he1.ranking != he2.ranking) {
    return he2.ranking - he1.ranking; //rankings are decreasing !!!!
  }
  // break the ties
  for(int i = 0; i < 5; i++) {
    if((he1.cards[i])->value != (he2.cards[i])->value){
      return (he1.cards[i])->value - (he2.cards[i])->value;
    }
  }
  return 0;
}

//You will write this function in Course 4.
//For now, we leave a prototype (and provide our
//implementation in eval-c4.o) so that the
//other functions we have provided can make
//use of get_match_counts.
unsigned * get_match_counts(deck_t * hand) {
  unsigned counters[52] = {0} ;
  for (size_t i = 0; i < hand->n_cards; i++) {
    unsigned value = hand->cards[i]->value;
    counters[value]++;
  }
  unsigned * result = malloc(hand->n_cards * sizeof(*result));
  for (size_t i = 0; i < hand->n_cards; i++) {
    result[i] = counters[hand->cards[i]->value];
  }
  return result;
}

// We provide the below functions.  You do NOT need to modify them
// In fact, you should not modify them!


//This function copies a straight starting at index "ind" from deck "from".
//This copies "count" cards (typically 5).
//into the card array "to"
//if "fs" is NUM_SUITS, then suits are ignored.
//if "fs" is any other value, a straight flush (of that suit) is copied.
void copy_straight(card_t ** to, deck_t *from, size_t ind, suit_t fs, size_t count) {
  assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
  unsigned nextv = from->cards[ind]->value;
  size_t to_ind = 0;
  while (count > 0) {
    assert(ind < from->n_cards);
    assert(nextv >= 2);
    assert(to_ind <5);
    if (from->cards[ind]->value == nextv &&
        (fs == NUM_SUITS || from->cards[ind]->suit == fs)){
      to[to_ind] = from->cards[ind];
      to_ind++;
      count--;
      nextv--;
    }
    ind++;
  }
}

//This looks for a straight (or straight flush if "fs" is not NUM_SUITS)
// in "hand".  It calls the student's is_straight_at for each possible
// index to do the work of detecting the straight.
// If one is found, copy_straight is used to copy the cards into
// "ans".
int find_straight(deck_t * hand, suit_t fs, hand_eval_t * ans) {
  if (hand->n_cards < 5){
    return 0;
  }
  for(size_t i = 0; i <= hand->n_cards -5; i++) {
    int x = is_straight_at(hand, i, fs);
    if (x != 0){
      if (x < 0) { //ace low straight
        assert(hand->cards[i]->value == VALUE_ACE &&
               (fs == NUM_SUITS || hand->cards[i]->suit == fs));
        ans->cards[4] = hand->cards[i];
        size_t cpind = i+1;
        while(hand->cards[cpind]->value != 5 ||
              !(fs==NUM_SUITS || hand->cards[cpind]->suit ==fs)){
          cpind++;
          assert(cpind < hand->n_cards);
        }
        copy_straight(ans->cards, hand, cpind, fs,4) ;
      }
      else {
        copy_straight(ans->cards, hand, i, fs,5);
      }
      return 1;
    }
  }
  return 0;
}

//This function puts all the hand evaluation logic together.
//This function is longer than we generally like to make functions,
//and is thus not so great for readability :(
hand_eval_t evaluate_hand(deck_t * hand) {
  suit_t fs = flush_suit(hand);
  hand_eval_t ans;
  if (fs != NUM_SUITS) {
    if(find_straight(hand, fs, &ans)) {
      ans.ranking = STRAIGHT_FLUSH;
      return ans;
    }
  }
  unsigned * match_counts = get_match_counts(hand);
  unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
  assert(n_of_a_kind <= 4);
  size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
  ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
  free(match_counts);
  if (n_of_a_kind == 4) { //4 of a kind
    return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
  }
  else if (n_of_a_kind == 3 && other_pair_idx >= 0) {     //full house
    ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
    ans.cards[3] = hand->cards[other_pair_idx];
    ans.cards[4] = hand->cards[other_pair_idx+1];
    return ans;
  }
  else if(fs != NUM_SUITS) { //flush
    ans.ranking = FLUSH;
    size_t copy_idx = 0;
    for(size_t i = 0; i < hand->n_cards;i++) {
      if (hand->cards[i]->suit == fs){
        ans.cards[copy_idx] = hand->cards[i];
        copy_idx++;
        if (copy_idx >=5){
          break;
        }
      }
    }
    return ans;
  }
  else if(find_straight(hand,NUM_SUITS, &ans)) {     //straight
    ans.ranking = STRAIGHT;
    return ans;
  }
  else if (n_of_a_kind == 3) { //3 of a kind
    return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
  }
  else if (other_pair_idx >=0) {     //two pair
    assert(n_of_a_kind ==2);
    ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
    ans.cards[2] = hand->cards[other_pair_idx];
    ans.cards[3] = hand->cards[other_pair_idx + 1];
    if (match_idx > 0) {
      ans.cards[4] = hand->cards[0];
    }
    else if (other_pair_idx > 2) {  //if match_idx is 0, first pair is in 01
      //if other_pair_idx > 2, then, e.g. A A K Q Q
      ans.cards[4] = hand->cards[2];
    }
    else {       //e.g., A A K K Q
      ans.cards[4] = hand->cards[4];
    }
    return ans;
  }
  else if (n_of_a_kind == 2) {
    return build_hand_from_match(hand, 2, PAIR, match_idx);
  }
  return build_hand_from_match(hand, 0, NOTHING, 0);
}

















