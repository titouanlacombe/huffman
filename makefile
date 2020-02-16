SRC=./src/
TMP=./tmp/
CC=gcc
CFLAGS=
LDFLAGS=-lm
EXEC=huffman
TESTARGS=

#------------------COMPIL------------------
huffman: main.o huffman.o binary_tree.o bin_file.o test.o
	$(CC) -o $(EXEC) $(TMP)main.o $(TMP)huffman.o $(TMP)binary_tree.o $(TMP)bin_file.o $(TMP)test.o $(LDFLAGS)

main.o: $(SRC)main.c
	$(CC) -o $(TMP)main.o -c $(SRC)main.c $(CFLAGS)

huffman.o: $(SRC)huffman.c $(SRC)huffman.h
	$(CC) -o $(TMP)huffman.o -c $(SRC)huffman.c $(CFLAGS)

binary_tree.o: $(SRC)binary_tree.c $(SRC)binary_tree.h
	$(CC) -o $(TMP)binary_tree.o -c $(SRC)binary_tree.c $(CFLAGS)

bin_file.o: $(SRC)bin_file.c $(SRC)bin_file.h
	$(CC) -o $(TMP)bin_file.o -c $(SRC)bin_file.c $(CFLAGS)

test.o: $(SRC)test.c $(SRC)test.h
	$(CC) -o $(TMP)test.o -c $(SRC)test.c $(CFLAGS)

#------------------TESTS------------------
test: compilation
	./$(EXEC) $(TESTARGS)

testbin: test.o
	$(CC) $(SRC)bin_file.c $(TMP)test.o
	./a.out
	rm -rf a.out

testtree: test.o
	$(CC) $(SRC)binary_tree.c $(TMP)test.o
	./a.out
	rm -rf a.out

testhuff: test.o bin_file.o binary_tree.o
	$(CC) $(SRC)huffman.c $(TMP)test.o $(TMP)bin_file.o $(TMP)binary_tree.o
	./a.out
	rm -rf a.out

#------------------OTHER------------------
clean:
	rm -rf $(TMP)*.o
