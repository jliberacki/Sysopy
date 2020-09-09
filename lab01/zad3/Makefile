CC = gcc -Wall

all: zad2_s zad2_sh zad3 zad2_O0 zad2_O2 zad2_Os

static_lib:
	$(CC) -c -Wall ../zad1/zad1.c -o ./zad1.o
	ar rcs libzad1.a zad1.o

shared_lib:
	$(CC) -fPIC -c ../zad1/zad1.c -o zad1.o
	$(CC) -shared -Wl,-soname,libzad1.so.1 -o libzad1.so.1.0.1 zad1.o
	ln -s libzad1.so.1.0.1 libzad1.so.1
	ln -s libzad1.so.1 libzad1.so

#Różne sposoby ładowania bibliotek
zad2_s: static_lib
	$(CC) -c ../zad2/zad2.c
	$(CC) zad2.o libzad1.a -o zad2_s

zad2_sh: shared_lib
	$(CC) ../zad2/zad2.c -o zad2_sh -lzad1 -L ./

zad3: static_lib
	$(CC) zad3.c -o zad3 -ldl

#Różne stopnie optymalizacji
zad2_O0: static_lib
	$(CC) -c ../zad2/zad2.c -O0
	$(CC) zad2.o libzad1.a -o zad2_O0 -O0

zad2_O2: static_lib
	$(CC) -c ../zad2/zad2.c -O2
	$(CC) zad2.o libzad1.a -o zad2_O2 -O2

zad2_Os: static_lib
	$(CC) -c ../zad2/zad2.c -Os
	$(CC) zad2.o libzad1.a -o zad2_Os -Os


test: all
	#Porownanie sposobów ładowanie bibliotek
	printf "\nStaytyczna biblioteka\n" > results3a.txt
	./zad2_s create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> results3a.txt
	printf "\nBiblioteka dzielona\n" >> results3a.txt
	./zad2_sh create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> results3a.txt
	printf "\nDynamiczne ładowanie\n" >> results3a.txt
	./zad3 create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> results3a.txt
	
	#Porownanie czasów zależnie od optymalizacji
	printf "\nOptymalizacja O0\n" > results3b.txt
	./zad2_O0 create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> results3b.txt
	printf "\nOptymalizacja O2\n" >> results3b.txt
	./zad2_O2 create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> results3b.txt
	printf "\nOptymalizacja Os\n" >> results3b.txt
	./zad2_Os create_table 20 search_directory / "*" test_big.txt load_file load_file load_file load_file load_file load_file load_file load_file remove_block 0 remove_block 1 remove_block 2 load_file load_file load_file load_file load_file load_file load_file remove_block 8 remove_block 5 remove_block 4 >> results3b.txt

	make clean_temp

clean_temp:
	rm -rf *.o test.txt test_medium.txt test_big.txt

clean: clean_temp
	rm -rf *.so *.a *.txt zad2_s zad2_sh zad3 libzad1.so.1 libzad1.so.1.0.1 zad2_O0 zad2_O2 zad2_Os