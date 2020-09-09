CC = gcc
CFLAGS = -Wall

all: server client	

shared: shared.h shared.c

server: server.c shared.c shared.h
	$(CC) $(CFLAGS) server.c shared.c -o server -lrt

client: client.c shared.c shared.h
	$(CC) $(CFLAGS) client.c shared.c -o client -lrt

clean:	
	rm -rf *.o server client

