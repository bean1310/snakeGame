CC = gcc
CFLAGS = -Wall

snakegame : functions.o main.o
	$(CC) $(CFLAGS) -lncurses -o $@ functions.o main.o

function.o : functions.c
	$(CC) -c $(CFLAGS) functions.c

main.o : main.c
	$(CC) -c $(CFLAGS) main.c
