#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"



int main(int argc, char ** argv) {
  //YOUR CODE GOES HERE
  unsigned num_trials = 10000;
  if (argc == 1) {
    printf("Usage : poker inputfilename\n");
    return EXIT_FAILURE;
  }
  if (argc == 3) { // overwrite num_trials
    num_trials = atoi(argv[2]);
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    printf("Could not open the input file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  future_cards_t * fc = malloc(sizeof(*fc));
  fc->n_decks = 0;
  fc->decks = NULL;
  size_t * n_hands = malloc(sizeof(*n_hands));
  *n_hands = 0;

  deck_t ** hands = read_input(f, n_hands, fc);
  deck_t * r_deck = build_remaining_deck(hands, *n_hands);

  unsigned * results = malloc((*n_hands + 1) * sizeof(*results));
  for (size_t i = 0; i < *n_hands + 1; i++){
    results[i] = 0;
  }

  for (unsigned n = 0; n < num_trials; n++) {
    shuffle(r_deck);
    future_cards_from_deck(r_deck, fc);

    // find the winning hand(s)
    size_t winning_index = 0;
    deck_t * winning_hand = hands[0];
    int tie = 0; // acts as a boolean !
    for (size_t i = 1; i < *n_hands; i++) {
      if (compare_hands(hands[i], winning_hand) > 0) {
        winning_hand = hands[i];
        winning_index = i;
        tie = 0;
      } else {
        if (compare_hands(hands[i], winning_hand) == 0) {
          tie = 1;
        }
      }
    }
        if (tie == 0) {
      results[winning_index]++;
    } else {
      results[*n_hands]++;
    }
  }

  for (size_t i = 0; i < *n_hands; i++) {
    printf("Hand %zu won %u / %u times (%.2f%%)\n", i, results[i], num_trials, results[i]*100.0/num_trials);
  }
  printf("And there were %u ties\n", results[*n_hands]);

  if (fclose(f) != 0) {
    printf("Could not close the input file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  for(size_t i = 0; i < *n_hands; i++){
    free_deck(hands[i]);
  }
  free(hands);
  free(n_hands);

  for(size_t i = 0; i < fc->n_decks; i++){
    deck_t d = fc->decks[i];
    for (size_t j = 0; j < d.n_cards; j++){
      d.cards[j] = NULL;
    }
    free((fc->decks[i]).cards);
  }
  free(fc->decks);
  free(fc);

    free_deck(r_deck);
  free(results);

  return EXIT_SUCCESS;
}
