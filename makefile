SRC=./src/
TMP=./tmp/
CC=gcc
CFLAGS=#-pg
LDFLAGS=-lm #-pg
EXEC=huffman

#------------------DEFAULT------------------
all: main

#------------------COMPIL------------------
main: main.o huffman.o binary_tree.o bin_file.o
	$(CC) -o $(EXEC) $(TMP)main.o $(TMP)huffman.o $(TMP)binary_tree.o $(TMP)bin_file.o $(LDFLAGS)

main.o: $(SRC)main.c
	$(CC) -o $(TMP)main.o -c $(SRC)main.c $(CFLAGS)

huffman.o: $(SRC)huffman.c $(SRC)huffman.h
	$(CC) -o $(TMP)huffman.o -c $(SRC)huffman.c $(CFLAGS)

binary_tree.o: $(SRC)binary_tree.c $(SRC)binary_tree.h
	$(CC) -o $(TMP)binary_tree.o -c $(SRC)binary_tree.c $(CFLAGS)

bin_file.o: $(SRC)bin_file.c $(SRC)bin_file.h
	$(CC) -o $(TMP)bin_file.o -c $(SRC)bin_file.c $(CFLAGS)

#------------------TESTS------------------
test_bin:
	$(CC) $(SRC)bin_file.c
	./a.out
	rm -rf a.out

test_tree:
	$(CC) $(SRC)binary_tree.c
	./a.out
	rm -rf a.out

test_huff: bin_file.o binary_tree.o
	$(CC) $(SRC)huffman.c $(TMP)bin_file.o $(TMP)binary_tree.o
	./a.out
	rm -rf a.out

test_main: main
	./$(EXEC)
	./$(EXEC) -h
	./$(EXEC) texts/medium.txt -o texts/test_main_out.bin
	./$(EXEC) texts/test_main_out.bin -o texts/test_main_out.txt

profile: main
	valgrind --tool=callgrind ./$(EXEC) texts/profile.txt -o texts/profile_out.bin
	kcachegrind callgrind.out.*

#------------------OTHER------------------
clean:
	rm -rf $(TMP)*.o
