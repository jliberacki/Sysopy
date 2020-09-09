CC = gcc -Wall 

all: static shared

static:
	$(CC) -c -Wall zad1.c -o zad1.o
	ar rcs libzad1.a zad1.o
	
shared:
	$(CC) -fPIC -c zad1.c
	$(CC) -shared -Wl,-soname,libzad1.so.1 -o libzad1.so zad1.o

clean:
	rm -rf *.o *.so *.a
