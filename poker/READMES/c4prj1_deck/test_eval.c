#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
#include "test_eval.h"

int main() {
  deck_t * d = malloc(sizeof(*d));
  d->n_cards = 0;
  d->cards = NULL;

  add_card_to(d, card_from_letters('K','s'));
  add_card_to(d, card_from_letters('K','h'));
  add_card_to(d, card_from_letters('Q','s'));
  add_card_to(d, card_from_letters('Q','h'));
  add_card_to(d, card_from_letters('0','s'));
  add_card_to(d, card_from_letters('9','d'));
  add_card_to(d, card_from_letters('9','s'));
  add_card_to(d, card_from_letters('9','h'));

  print_hand(d);
  printf("\n");
  unsigned * result = get_match_counts(d);

  assert(sizeof(result) == d->n_cards);

  for(size_t i = 0; i < d->n_cards; i++) {
    printf("%u ",result[i]);
  }
  printf("\n");
  free(result);
  free_deck(d);
  return 0;
}