CFLAGS=-std=gnu99 -pedantic -Wall
all: main test_input
test_input: test_input.o input.o future.o deck.o cards.o
        gcc -o test_input test_input.o input.o future.o deck.o cards.o
main: main.o future.o deck.o cards.o
        gcc -o main main.o future.o deck.o cards.o
test_input.o: test_input.c input.h future.h
        gcc $(CFLAGS) -c test_input.c
input.o: input.c input.h
        gcc $(CFLAGS) -c input.c
main.o: main.c deck.h future.h
        gcc $(CFLAGS) -c main.c
future.o: future.c future.h
        gcc $(CFLAGS) -c future.c
deck.o: deck.c deck.h
        gcc $(CFLAGS) -c deck.c
cards.o: cards.c cards.h
        gcc $(CFLAGS) -c cards.c
.PHONY: clean
clean:
        rm -f main test_input *.o *.c~ *.h~ Makefile~