all: arbre_binaire.o bin_file.o huffman

arbre_binaire.o: arbre_binaire.c arbre_binaire.h
	gcc -c arbre_binaire.c

bin_file.o: bin_file.c bin_file.h
	gcc -c bin_file.c

huffman: huffman.c huffman.h arbre_binaire.c arbre_binaire.h bin_file.c bin_file.h
	gcc -o huffman huffman.c arbre_binaire.c bin_file.c