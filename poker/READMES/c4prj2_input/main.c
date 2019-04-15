#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
#include "future.h"

int main() {
  deck_t * d1 = malloc(sizeof(*d1));
  d1->n_cards = 0;
  d1->cards = NULL;
  card_t c1 = card_from_letters('K', 's');
  add_card_to(d1, c1);
  card_t * ec1 = add_empty_card(d1);
  print_hand(d1);
  printf("\n");

  deck_t * d2 = malloc(sizeof(*d2));
  d2->n_cards = 0;
  d2->cards = NULL;
  card_t c2 = card_from_letters('Q', 'd');
  add_card_to(d2, c2);
  card_t * ec2 = add_empty_card(d2);
  print_hand(d2);
  printf("\n");

  deck_t * d3 = malloc(sizeof(*d3));
  d3->n_cards = 0;
  d3->cards = NULL;
  card_t c3 = card_from_letters('J', 'h');
  add_card_to(d3, c3);
  card_t * ec3 = add_empty_card(d3);
  card_t * ec4 = add_empty_card(d3);
  print_hand(d3);
  printf("\n");


  printf("***** Deck to draw cards from (d4 draw cards 5c, 0c, Jc) *****\n");

  deck_t * d4 = malloc(sizeof(*d4));
  d4->n_cards = 0;
  d4->cards = NULL;
  card_t c4 = card_from_letters('5', 'c');
  add_card_to(d4, c4);
  card_t c5 = card_from_letters('0','c');
  add_card_to(d4, c5);
  card_t c6 = card_from_letters('J', 'c');
  add_card_to(d4, c6);
  printf("deck (d4) to draw from : ");
  print_hand(d4);
  printf("\n");

  printf("\n********* testing 2 future_cards_from_deck ********\n\n");

  future_cards_t * fc = malloc(sizeof(*fc));
  fc->n_decks = 0;
  fc->decks = NULL;

  add_future_card(fc, 0, ec1);
  add_future_card(fc, 0, ec2);
  add_future_card(fc, 1, ec3);
  add_future_card(fc, 2, ec4);

    printf("future cards : \n");
  for(int i = 0; i < 3; i++) {
    print_hand(&fc->decks[i]);
    printf("\n");
  }

  assert((fc->decks[0]).n_cards == 2);
  assert((fc->decks[1]).n_cards == 1);
  assert((fc->decks[2]).n_cards == 1);


  future_cards_from_deck(d4, fc);
  print_hand(d1);
  printf("\n");
  print_hand(d2);
  printf("\n");
  print_hand(d3);
  printf("\n");

  // Free : clean up everything
  free_deck(d4);
  free_deck(d1);
  free_deck(d2);
  free_deck(d3);

  for(size_t i = 0; i < fc->n_decks; i++){
    deck_t d = fc->decks[i];
    for (size_t j = 0; j < d.n_cards; j++){
      d.cards[j] = NULL;
    }
    free((fc->decks[i]).cards);
  }

    free(fc->decks);
  free(fc);


  printf("Ok : all tests passed!\n");

  return 0;
}