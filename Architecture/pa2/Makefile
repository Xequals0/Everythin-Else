FLAGS = -Wall -g
CC = gcc

all: calc.o format.o
	$(CC) $(FLAGS) -o calc calc.o
	$(CC) $(FLAGS) -o format format.o

calc.o: calc.c calc.h
	$(CC) $(FLAGS) -c calc.c

format.o: format.c format.h
	$(CC) $(FLAGS) -c format.c

clean:
	rm -f all calc.o format.o calc format