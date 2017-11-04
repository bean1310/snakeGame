snake : functions.o main.o
	gcc -Wall -lncurses -o $@ functions.o main.o

function.o : functions.c
	gcc -Wall -c $(CFLAGS) functions.c

main.o : main.c
	gcc -Wall -c main.c