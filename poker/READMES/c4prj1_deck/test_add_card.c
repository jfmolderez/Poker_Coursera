#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
#include "eval.h"


int main() {
  // test add_card_to
  deck_t * d = malloc(sizeof(*d));
  d->n_cards = 0;
  d->cards = NULL;
  printf("\n**** testing add_card_to ****\n");
  card_t c1 = card_from_num(0);
  card_t c2 = card_from_num(1);
  card_t c3 = card_from_num(2);
  add_card_to(d, c1);
  add_card_to(d, c2);
  add_card_to(d, c3);

  assert(deck_contains(d, c1));
  print_hand(d);
  printf("\n");
  printf("# cards in deck : %lu\n", d->n_cards);

  // test add_empty_card
  printf("\n**** test add empty card ****\n");
  card_t * ecard1 = add_empty_card(d);
  card_t * ecard2 = add_empty_card(d);
  print_hand(d);
  printf("\n");
  print_card(*ecard1);
  printf("\n");
  printf("# cards in deck : %lu\n", d->n_cards);

  free_deck(d);
  free(ecard1);
  free(ecard2);

  // test make_deck_exclude
  printf("\n**** test make_deck_exclude ****\n");


  deck_t * excluded = malloc(sizeof(*excluded));
  excluded->n_cards = 0;
  excluded->cards = NULL;
  card_t c4 = card_from_num(51);
  card_t c5 = card_from_num(0);
  card_t c6 = card_from_num(26);

  add_card_to(excluded, c4);
  add_card_to(excluded, c5);
  add_card_to(excluded, c6);

  deck_t * d0 = make_deck_exclude(excluded);
  assert(d0->n_cards == 49);
  print_hand(d0);
  printf("\n");

    deck_t * empty_deck = malloc(sizeof(*empty_deck));
  empty_deck->n_cards = 0;
  empty_deck->cards = NULL;
  deck_t * d1 = make_deck_exclude(empty_deck);
  assert(d1->n_cards == 52);
  print_hand(d1);
  printf("\n");

  deck_t * d2 = make_deck_exclude(d0);
  assert(d2->n_cards == 3);
  print_hand(d2);
  printf("\n");

  free_deck(empty_deck);
  free_deck(excluded);
  free_deck(d0);
  free_deck(d1);
  free_deck(d2);

  // test build_remaining_deck
  printf("\n**** test build_remaining_deck ****\n");
  deck_t * h1 = malloc(sizeof(*h1));
  h1->n_cards = 0;
  h1->cards = NULL;
  add_card_to(h1, c4);
  add_card_to(h1, c5);
  card_t * ec1 = add_empty_card(h1);
  card_t * ec2 = add_empty_card(h1);
  printf("*** h1 : ");
  print_hand(h1);
  printf("\n");

  card_t c7 = card_from_num(50);
  card_t c8 = card_from_num(1);
  deck_t * h2 = malloc(sizeof(*h2));
  h2->n_cards = 0;
  h2->cards = NULL;
  add_card_to(h2, c7);
  add_card_to(h2, c8);
  card_t * ec3 = add_empty_card(h2);
  printf("*** h2 : ");
  print_hand(h2);
  printf("\n");

  deck_t ** hands = malloc(2 * sizeof(*hands));
  hands[0] = h1;
  hands[1] = h2;
  deck_t * result = build_remaining_deck(hands, 2);
  assert(result->n_cards == 48);
  print_hand(result);
  printf("\n");

  free(ec1);
  free(ec2);
  free(ec3);
  free_deck(h1);
  free_deck(h2);
  free(hands);
  free_deck(result);



  return 0;
}







