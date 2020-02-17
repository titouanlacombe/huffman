#include "huffman.h"

// For each unique char in input_text create a leaf in leaf with 
// elt as the unique char and weight as it's number of aparition in input_text
int huff_build_leaves(char *input_text, Binary_tree *leaves[]) {
	int i, index, char_freq[UCHAR_MAX + 1];

	for (i = 0; i < UCHAR_MAX + 1; i++) {
		char_freq[i] = 0;
	}
	
	i = 0;
	while (input_text[i] != '\0') {
		index = (int)input_text[i] - CHAR_MIN;
		char_freq[index]++;
		i++;
	}
	
	index = 0;
	for (i = 0; i < UCHAR_MAX + 1; i++) {
		if (char_freq[i] > 0) {
			leaves[index] = Btree_create_leaf((char)(i + CHAR_MIN), char_freq[i]);
			index++;
		}
	}
}

// sort the leaves by a descending weight
// algorithm: Selection sort (https://en.wikipedia.org/wiki/Selection_sort)
void huff_sort_leaves(Binary_tree *leaves[], int nb_leaves) {
	int i, j, max;
	Binary_tree *tmp;

	for (i = 0; i < nb_leaves - 1; i++) {
		max = i;
		for (j = i + 1; j < nb_leaves; j++) {
			if (leaves[j]->weight > leaves[max]->weight) {
				max = j;
			}
		}
		tmp = leaves[i];
		leaves[i] = leaves[max];
		leaves[max] = tmp;
	}
}

// insert a tree in the leaves array to conserve the sorting
void huff_insert_tree(Binary_tree *leaves[], Binary_tree *tree, int nb_leaves) {
	int i, j;

	i = nb_leaves - 1;
	while (leaves[i]->weight < tree->weight && i > 0) {
		i--;
	}

	for (j = nb_leaves; j > i; j--) {
		leaves[j] = leaves[j - 1];
	}
	leaves[i] = tree;
}

// Build the tree from the leave with the huffman algorithm
Binary_tree *huff_build_tree(Binary_tree *leaves[], int nb_leaves) {
	Binary_tree *tree1, *tree2, *tree3;

	huff_sort_leaves(leaves, nb_leaves);

	while (nb_leaves > 1) {
		// take the last two (least weight)
		tree1 = leaves[nb_leaves - 1];
		tree2 = leaves[nb_leaves - 2];

		// fusion them
		tree3 = Btree_create_node(tree1, tree2);

		// insert the result
		huff_insert_tree(leaves, tree3, nb_leaves);
		nb_leaves--;
	}
	return leaves[0];
}

// Serialize the tree and return a char*
char *huff_serialize_tree(Binary_tree *tree) {

}

// Deserialize the tree from the char* given in arg and return the tree
Binary_tree *huff_deserialize_tree(char* serial) {
	
}

// Encode the input_text with the huffman compression using the leaves and their paths
char *huff_tree_encode(char *input_text, Binary_tree *leaves[]) {

}

void huff_tree_decode() {
	
}

// Write in the output file the encoded_text and the huffman_serial
void huff_pack_output(Bin_file *output, char *encoded_text, char *huffman_serial) {

}

void huff_unpack_input(Bin_file *input, char *encoded_text, char *huffman_serial) {

}

// Encode the input_path file and puts the results in the output file
// Errors codes:
// -1: Unable to open input_path
// -2: Unable to open output_path
int huff_encode(char *input_path, char *output_path, int *input_size, int *output_size) {
	int nb_leaves;
	char *input_text, *huffman_serial, *encoded_text;
	Bin_file *input, *output;
	Binary_tree *leaves[UCHAR_MAX + 1], *huffman_tree;

	input = bin_open(input_path, 'r');
	if (!input) {
		return -1;
	}
	output = bin_open(output_path, 'w');
	if (!output) {
		return -2;
	}

	// Get input
	bin_file_to_string(input, input_text);

	// Construct huffman tree
	nb_leaves = huff_build_leaves(input_text, leaves);
	huffman_tree = huff_build_tree(leaves, nb_leaves);
	Btree_print(huffman_tree);
	
	// Encode text
	Btree_set_paths(huffman_tree, nb_leaves + 1);
	encoded_text = huff_tree_encode(input_text, leaves);

	// Construct output
	huffman_serial = huff_serialize_tree(huffman_tree);
	huff_pack_output(output, encoded_text, huffman_serial);

	// free and close
	free(input_text);
	free(huffman_serial);
	free(encoded_text);
	Btree_free(huffman_tree);
	*input_size = bin_close(input);
	*output_size = bin_close(output);

	return 0;
}

void huff_decode() {

}

int main() {
	int code, input_size, output_size;

	code = huff_encode("./texts/easy.txt", "./texts/test_huff.bin", &input_size, &output_size);
	printf("code: %i\n", code);
	printf("input_size: %i\n", input_size);
	printf("output_size: %i\n", output_size);

	return 0;
}
