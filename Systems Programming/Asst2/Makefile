FLAGS = -Wall -g -fsanitize=address
CC = gcc

all: invertedIndex

invertedIndex: asst2proto.o filesorter.o list.o
	$(CC) $(FLAGS) -o invertedIndex asst2proto.o filesorter.o list.o

asst2proto.o: asst2proto.c
	$(CC) $(FLAGS) -c asst2proto.c

filesorter.o: filesorter.c filesorter.h
	$(CC) $(FLAGS) -c filesorter.c

list.o: list.c list.h
	$(CC) $(FLAGS) -c list.c

clean:
	rm -rf *.o invertedIndex

