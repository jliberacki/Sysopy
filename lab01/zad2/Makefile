CC = gcc -Wall

all: zad2

static_lib:
	$(CC) -c -Wall ../zad1/zad1.c -o ./zad1.o
	ar rcs libzad1.a zad1.o

zad2: static_lib
	$(CC) -c zad2.c
	$(CC) zad2.o libzad1.a -o zad2

test: all
	printf "Small\n" > raport2.txt
	./zad2 create_table 2 search_directory ~/test "*" test.txt load_file >> raport2.txt
	printf "\nMedium\n" >> raport2.txt
	./zad2 create_table 10 search_directory /usr/bin "*" test_medium.txt load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 >> raport2.txt
	printf "\nLarge\n" >> raport2.txt
	./zad2 create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> raport2.txt
	
clean:
	rm -rf *.o *.so *.a *.txt zad2 zad2_d