CC = gcc
CFLAGS = -Wall

snakegame : functions.o main.o
	$(CC) $(CFLAGS) -o $@ functions.o main.o -lncurses

function.o : functions.c functions.h
	$(CC) -c $(CFLAGS) functions.c

main.o : main.c functions.h
	$(CC) -c $(CFLAGS) main.c
