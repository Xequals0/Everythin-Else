FLAGS = -Wall -g -fsanitize=address
CC = gcc

all: memgrind

memgrind: memgrind.o mymalloc.o
	$(CC) $(FLAGS) -o memgrind memgrind.o mymalloc.o

memgrind.o: memgrind.c
	$(CC) $(FLAGS) -c memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(FLAGS) -c mymalloc.c

clean:
	rm -rf *.o memgrind
