FLAGS = -Wall -m32 -g
CC = gcc

formula: nCr.o formula.o
	$(CC) $(FLAGS) nCr.o formula.o -o formula

formula.o: nCr.o nCr.h formula.c formula.h
	$(CC) $(FLAGS) -c formula.c -o formula.o

nCr.o: nCr.s
	$(CC) $(FLAGS) -c nCr.s -o nCr.o

clean:
	rm -rf *.o formula
