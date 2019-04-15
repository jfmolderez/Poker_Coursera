#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "input.h"

deck_t * hand_from_string(const char * str, future_cards_t * fc) {
  int i = 0;
  int j = 0;
  deck_t * d = malloc(sizeof(*d));
  d->n_cards = 0;
  d->cards = 0;
  while (str[i] != '\0') {
    if (isalnum(str[i]) && isalpha(str[i+1])) {
      card_t c = card_from_letters(str[i], str[i+1]);
      assert_card_valid(c);
      add_card_to(d, c);
      i = i + 2;
      j++;
    } else {
      if (str[i] == '?') {
        assert(isdigit(str[i+1]));
        int k = 0;
        char * index = NULL;
        while(isdigit(str[i + k + 1])) {
          index = realloc(index, (k + 1) * sizeof(*index));
          index[k] = str[i + k + 1];
          k++;
        }
        index = realloc(index, (k + 1) * sizeof(*index));
        index[k] = '\0';
        int ix = atoi(index);
        free(index);
        card_t * ec = add_empty_card(d);
        add_future_card(fc, ix, ec);
        i = i + 2;
        j++;
      } else { // white space : skip
        i++;
      }
    }
  }

  if (j < 5) {
    fprintf(stderr, "At least 5 cards per input line !\n");
    free_deck(d);
    exit(EXIT_FAILURE);
  }

  return d;
}

deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t *fc) {

  deck_t ** decks = NULL;

  size_t sz = 0;
  ssize_t len = 0;
  char * line = NULL;
  size_t i = 0;
  deck_t * hand = NULL;

  while((len = getline(&line, &sz, f)) >= 0){
    if (len > 1) {
      hand = hand_from_string(line, fc);
      decks = realloc(decks, (i + 1)*sizeof(*decks));
      decks[i] = hand;
      hand = NULL;
      free(line);
      line = NULL;
      i++;
    }
  }
  free(line);
  *n_hands = i;
  return decks;
}
