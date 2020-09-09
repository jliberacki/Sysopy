CC = gcc -Wall -std=c11 -ggdb

both:
	make master
	make slave

master:
	$(CC) -o master master.c -I .

slave:
	$(CC) -o slave slave.c -I .

test_master:
	./master fifo

test_slave:
	./slave fifo 5

clean:
	rm -f slave master fifo