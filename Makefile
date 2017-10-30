snake : functions.o main.o
	gcc -lncurses -o $@ functions.o main.o

function.o : functions.c
	gcc -c $(CFLAGS) functions.c

main.o : main.c
	gcc -c main.c