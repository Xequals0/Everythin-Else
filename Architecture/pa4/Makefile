FLAGS = -Wall -g
CC = gcc

all: y86emul.o y86dis.o
	$(CC) $(FLAGS) -o y86emul y86emul.o	
y86emul: y86emul.c y86emul.h
	$(CC) $(FLAGS) -c y86emul.c
	$(CC) $(FLAGS) -o y86emul y86emul.o
clean:
	rm -f all y86emul.o y86dis.o y86emul y86dis