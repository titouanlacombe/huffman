#include "huffman.h"

void huff_encode();
void huff_decode();

void huff_build_tree();
void huff_get_tree_leaves();

// Serialize the tree and return a char*
char *huff_serialize_tree(Binary_tree *tree) {

}

// Deserialize the tree from the char* given in arg and return the tree aux
Binary_tree *huff_deserialize_tree_aux(char* s, int *n) {

}

// Deserialize the tree from the char* given in arg and return the tree
Binary_tree *huff_deserialize_tree(char* serial) {
	int n = 0;
	Binary_tree *tree;
	tree = huff_deserialize_tree_aux(serial, &n);
	return tree;
}

void huff_tree_encode();
void huff_tree_decode();

/*
int main()
{
	
	return 0;
}*/
