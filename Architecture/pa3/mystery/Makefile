FLAGS = -Wall -m32 -g
CC = gcc

mystery: mystery.o
	$(CC) $(FLAGS) mystery.o -o mystery
	
mystery.o: mystery.c mystery.h
	$(CC) $(FLAGS) -c mystery.c 

clean:
	rm -rf *.o mystery
