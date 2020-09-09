CC = gcc -Wall -std=c11 -ggdb

main:
	$(CC) -o monitor monitor.c -I .
	make test

test:
	./monitor "./lista" 10 0

generator:
	$(CC) -o tester tester.c -I .
	make generate

generate:
	./tester tmp1.txt 0.5 1 3

clean:
	rm -r tmp*
	rm -f tester monitor
	touch tmp1.txt
	touch tmp2.txt