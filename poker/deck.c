#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"

void print_hand(deck_t * hand){
  card_t ** ptr = hand->cards;
  size_t n = hand->n_cards;

  for(size_t i = 0; i < n; i++) {
    card_t * ptrc = *ptr;
    print_card(*ptrc);
    printf(" ");
    ptr++;
  }
}

int deck_contains(deck_t * d, card_t c) {
  card_t ** ptr = d->cards;
  size_t n = d->n_cards;

  for(size_t i = 0; i < n; i++) {
    card_t * ptrc = *(ptr + i);
    if (ptrc->value == c.value && ptrc->suit == c.suit) {
      return 1;
    }
  }
  return 0;
}


void shuffle(deck_t * d){
  card_t ** ptr = d->cards;
  size_t n = d->n_cards;
  card_t * temp;
  int r;

  for(size_t i = n; i > 1; i--) {
    r = random() % i;
    temp = *(ptr + r);
    *(ptr + r) = *(ptr + i - 1);
    *(ptr + i - 1) = temp;
  }
}

void assert_full_deck(deck_t * d) {
  card_t ** ptr = d->cards;
  size_t n = d->n_cards;
  card_t c1, c2;

  for(size_t i = 0; i < n - 1; i++) {
    c1 = **(ptr + i);
    for(size_t j = i+1; j < n; j++) {
      c2 = **(ptr + j);
      assert(value_letter(c1) != value_letter(c2) || suit_letter(c1) != suit_letter(c2));
    }
  }
}

void add_card_to(deck_t * deck, card_t c) {
  deck->n_cards++;
  deck->cards = realloc(deck->cards, deck->n_cards*sizeof(*deck->cards));
  card_t * cptr = malloc(sizeof(*cptr));
  cptr->suit = c.suit;
  cptr->value= c.value;
  deck->cards[deck->n_cards-1] = cptr;
}

card_t * add_empty_card(deck_t * deck) {
  card_t * cptr = malloc(sizeof(*cptr));
  cptr->suit = NUM_SUITS;
  cptr->value = 0;
  add_card_to(deck, *cptr);
  free(cptr);
  return deck->cards[deck->n_cards-1];
}

deck_t * make_deck_exclude(deck_t * excluded_cards) {
  deck_t * d = malloc(sizeof(*d));
  d->n_cards = 0;
  d->cards = NULL;
  for(unsigned i = 0; i < 52; i++) {
    card_t c = card_from_num(i);
    if (!deck_contains(excluded_cards, c)){
      add_card_to(d, c);
    }
  }
  return d;
}

deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands) {
  deck_t * excluded = malloc(sizeof(*excluded));
  excluded->n_cards = 0;
  excluded->cards = NULL;

  for(size_t i = 0; i < n_hands; i++){
    size_t n = hands[i]->n_cards;
    for(size_t j = 0; j < n; j++){
      card_t * cptr = hands[i]->cards[j];
      if (cptr->value >= 2 && cptr->value <= VALUE_ACE && cptr->suit >= SPADES && cptr->suit <= CLUBS){
        add_card_to(excluded, *cptr);
      }
    }
  }
  deck_t * result =  make_deck_exclude(excluded);
  free_deck(excluded);
  return result;
}

void free_deck(deck_t * deck) {
  for (size_t i = 0; i < deck->n_cards; i++) {
    free(deck->cards[i]);
  }
  free(deck->cards);
  free(deck);
}
