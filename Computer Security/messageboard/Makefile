FLAGS = -Wall -g -fsanitize=address
CC = gcc

all: client server

client: client.o
	$(CC) $(FLAGS) -o client client.o

server: server.o
	$(CC) $(FLAGS) -o server server.o

client.o: client.c
	$(CC) $(FLAGS) -c client.c

server.o: server.c
	$(CC) $(FLAGS) -c server.c

clean:
	rm -rf *.o client server
