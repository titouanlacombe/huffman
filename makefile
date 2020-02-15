SRC=./src/
TMP=./tmp/
CC=gcc
CFLAGS=
LDFLAGS=-lm
EXEC=huffman
TESTARGS=

compilation: main.o
	$(CC) -o $(EXE
	C) $(TMP)main.o $(TMP)test.o $(LDFLAGS)

main.o: $(SRC)main.c test.o
	$(CC) -o $(TMP)main.o -c $(SRC)main.c $(CFLAGS)

test.o: $(SRC)test.c $(SRC)test.h
	$(CC) -o $(TMP)test.o -c $(SRC)test.c $(CFLAGS)

test: compilation
	./$(EXEC) $(TESTARGS)

clean:
	rm -rf $(TMP)*.o

testbin:
	$(CC) $(SRC)bin_file.c
	./a.out
	rm -rf a.out