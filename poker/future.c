#include <stdio.h>
#include <stdlib.h>
#include "future.h"
#include "deck.h"

void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
  if (fc->n_decks <= index) {
    fc->decks = realloc(fc->decks, (index + 1)*sizeof(deck_t));
    for(size_t i = fc->n_decks; i <= index; i++){
      (fc->decks[i]).n_cards = 0;
      (fc->decks[i]).cards = NULL;
    }
    fc->n_decks = index + 1;
  }
  size_t nc = (fc->decks[index]).n_cards;
  (fc->decks[index]).cards = realloc((fc->decks[index]).cards, (nc + 1)*sizeof(ptr));
  (fc->decks[index]).cards[nc] = ptr;
  (fc->decks[index]).n_cards++;
}

void future_cards_from_deck(deck_t * deck, future_cards_t * fc) {
  if(deck->n_cards < fc->n_decks) {
    fprintf(stderr, "size of deck (%lu) is too small vs size of future cards (%lu)", deck->n_cards, fc->n_decks);
    exit(EXIT_FAILURE);
  }
  for(size_t i = 0; i < fc->n_decks; i++) {
    card_t * cptr = deck->cards[i]; // card to be drawn
    deck_t d = fc->decks[i];
    for(size_t j = 0; j < d.n_cards; j++) {
      d.cards[j]->value = cptr->value;
      d.cards[j]->suit = cptr->suit;
    }
  }
}
