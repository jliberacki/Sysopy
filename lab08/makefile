CC = gcc -Wall -std=c11 -ggdb -lpthread -lm

all: clean main

main:
	$(CC) -o main main.c -I .

clean:
	rm -f main
test:
	./main "1" "interleaved" "baboon.ascii.pgm" "filter" "filtered.pgm"
