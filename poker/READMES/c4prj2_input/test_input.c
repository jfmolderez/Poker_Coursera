#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "deck.h"
#include "future.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage : test inputfilename\n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Could not open the file %s", argv[1]);
    exit(EXIT_FAILURE);
  }

  future_cards_t * fc = malloc(sizeof(*fc));
  fc->n_decks = 0;
  fc->decks = NULL;
  size_t * n_hands = malloc(sizeof(*n_hands));
  *n_hands = 0;

  deck_t ** hands = read_input(f, n_hands, fc);

  printf("Number of hands = %lu\n", *n_hands);
  printf("Number of unknowns = %lu\n", fc->n_decks);
  for(size_t i = 0; i < *n_hands; i++) {
    print_hand(hands[i]);
    printf("\n");
  }

  if (fclose(f) != 0) {
    fprintf(stderr, "Could not close the file %s", argv[1]);
    exit(EXIT_FAILURE);
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


  return EXIT_SUCCESS;
}