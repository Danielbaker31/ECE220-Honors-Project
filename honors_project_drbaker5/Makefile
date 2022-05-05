CC=gcc
CFLAGS=-g -std=c99 -g -Wall -Werror

all: battleship

battleship: main.o play.o
	$(CC) $(CFLAGS) main.o play.o -lm -o battleship

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

play.o: play.c
	$(CC) $(CFLAGS) -c play.c

clean:
	rm -f play.o main.o battleship
