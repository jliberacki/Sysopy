CC = gcc -Wall -std=gnu99

all: server client


server:
	$(CC) server.c common.h -o server -lpthread


client:
	$(CC) client.c common.h -o client -lpthread


clean:
	rm -rf client server
