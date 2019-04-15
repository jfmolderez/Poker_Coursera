CFLAGS=-std=gnu99 -pedantic -Wall
all: test_add_card test_eval
test_eval: test_eval.o eval.o deck.o cards.o
        gcc -o test_eval test_eval.o eval.o deck.o cards.o
test_add_card: test_add_card.o deck.o cards.o
        gcc -o test_add_card test_add_card.o deck.o cards.o
test_eval.o: test_eval.c test_eval.h
        gcc $(CFLAGS) -c test_eval.c
eval.o: eval.c eval.h
        gcc $(CFLAGS) -c eval.c
test_add_card.o: test_add_card.c
        gcc $(CFLAGS) -c test_add_card.c
deck.o: deck.c deck.h
        gcc $(CFLAGS) -c deck.c
cards.o: cards.c cards.h
        gcc $(CFLAGS) -c cards.c
.PHONY: clean
clean:
        rm -f test_eval test_add_card *.o *.c~ *.h~ Makefile~