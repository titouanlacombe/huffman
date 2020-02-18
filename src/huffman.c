#include "huffman.h"

#define SERIAL_NODE 1
#define SERIAL_LEAF 2
#define SERIAL_END 3

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
	return index;
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
	Binary_tree *tree1, *tree2, *tree3, *leaves_copy[nb_leaves];

	huff_sort_leaves(leaves, nb_leaves);
	for (int i = 0; i < nb_leaves; i++) {
		leaves_copy[i] = leaves[i];
	}
	
	while (nb_leaves > 1) {
		// take the last two (least weight)
		tree1 = leaves_copy[nb_leaves - 1];
		tree2 = leaves_copy[nb_leaves - 2];

		// fusion them
		tree3 = Btree_create_node(tree1, tree2);
		tree3->weight = tree1->weight + tree2->weight;

		// insert the result
		huff_insert_tree(leaves_copy, tree3, nb_leaves);
		nb_leaves--;
	}
	return leaves_copy[0];
}

int huff_get_encoded_len(char *input_text, Binary_tree *leaves[], int nb_leaves) {
	int sum;

	sum = 0;
	for (int i = 0; i < nb_leaves; i++) {
		sum += leaves[i]->weight * strlen(leaves[i]->path);
	}
	return sum;
}

Binary_tree *huff_find_leaf(char elt, Binary_tree *leaves[], int nb_leaves) {
	for (int i = 0; i < nb_leaves; i++) {
		if (leaves[i]->elt == elt) {
			return leaves[i];
		}
	}
}

void strappend(char *dest, char *str, int start) {
	for (int i = 0; i < strlen(str); i++) {
		dest[i + start] = str[i];
	}
}

// Encode the input_text with the huffman compression using the leaves and their paths
char *huff_tree_encode(char *input_text, Binary_tree *leaves[], int nb_leaves) {
	Binary_tree *leaf;
	char *encoded_text;
	int encoded_text_i, encoded_length;

	encoded_length = huff_get_encoded_len(input_text, leaves, nb_leaves);
	encoded_text = malloc(encoded_length * sizeof(char) + 1);

	encoded_text_i = 0;
	for (int i = 0; i < strlen(input_text); i++) {
		leaf = huff_find_leaf(input_text[i], leaves, nb_leaves);
		strappend(encoded_text, leaf->path, encoded_text_i);
		encoded_text_i += strlen(leaf->path);
	}
	input_text[encoded_text_i] = '\0';
	
	return encoded_text;
}

void huff_tree_decode() {
	
}

// Serialize the tree and return a char* aux
void huff_serialize_tree_aux(Binary_tree *tree, char *serial_tree, int *serial_tree_i) {
	if (Btree_is_leaf(tree)) {
		serial_tree[*serial_tree_i] = SERIAL_LEAF;
		(*serial_tree_i)++;
		serial_tree[*serial_tree_i] = tree->elt;
		(*serial_tree_i)++;
	}
	else {
		serial_tree[*serial_tree_i] = SERIAL_NODE;
		(*serial_tree_i)++;
		huff_serialize_tree_aux(tree->left_child, serial_tree, serial_tree_i);
		huff_serialize_tree_aux(tree->right_child, serial_tree, serial_tree_i);
	}
}

// Serialize the tree and return a char*
// recursive
// node->0
// leaf->1 + 'elt'
char *huff_serialize_tree(Binary_tree *tree, char *serial_tree) {
	int i = 0;
	huff_serialize_tree_aux(tree, serial_tree, &i);
	serial_tree[i] = SERIAL_END;
	i++;
	return serial_tree;
}

// Deserialize the tree from the char* given in arg and return the tree
Binary_tree *huff_deserialize_tree(char* serial) {
	
}

// Write in the output file the encoded_text and the huffman_serial
void huff_pack_output(Bin_file *output, char *encoded_text, char *huffman_serial) {
	int i = 0;
	while (huffman_serial[i] != SERIAL_END) {
		bin_write_byte(output, huffman_serial[i]);
		i++;
	}

	for (i = 0; i < strlen(encoded_text); i++) {
		bin_write_bit(output, encoded_text[i]);
	}
}

void huff_unpack_input(char *input_text, char *encoded_text, char *huffman_serial) {
	encoded_text = malloc(strlen(input_text)*sizeof(char));
	huffman_serial = malloc(strlen(input_text)*sizeof(char));

	int i = 0;
	while (input_text[i] != SERIAL_END) {
		huffman_serial[i] = input_text[i];
		i++;
	}
	huffman_serial[i] = '\0';
	
	int j = 0;
	while (i < strlen(input_text)) {
		encoded_text[j] = input_text[i];
		i++;
		j++;
	}
}

void print_serial(char *huffman_serial) {
	int i = 0;

	printf("Serial: ");
	while (huffman_serial[i] != SERIAL_END) {
		if (huffman_serial[i] == SERIAL_NODE) {
			printf("node|");
		} else if (huffman_serial[i] == SERIAL_LEAF) {
			printf("leaf|");
		} else {
			printf("%c|", huffman_serial[i]);
		}
		i++;
	}
	printf("end\n");
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
	input_text = bin_file_to_string(input);
	// printf("Input: %s\n", input_text);

	// Construct huffman tree
	nb_leaves = huff_build_leaves(input_text, leaves);	
	huffman_tree = huff_build_tree(leaves, nb_leaves);
	
	// Encode text
	Btree_set_paths(huffman_tree, nb_leaves + 1);
	
	encoded_text = huff_tree_encode(input_text, leaves, nb_leaves);
	printf("Encoded: %s\n", encoded_text);

	// Serialize tree
	Binary_tree *nodes[nb_leaves];
	int nb_nodes = Btree_get_nodes(huffman_tree, nodes);
	int size_of_serial = 2 * nb_leaves + nb_nodes + 1;
	huffman_serial = malloc(size_of_serial * sizeof(char));
	huff_serialize_tree(huffman_tree, huffman_serial);
	print_serial(huffman_serial);
	
	// Construct output
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

int huff_decode(char *input_path, char *output_path, int *input_size, int *output_size) {
	char *huffman_serial, *decoded_text, *encoded_text, *input_text;
	Bin_file *input, *output;
	Binary_tree *huffman_tree;

	input = bin_open(input_path, 'r');
	if (!input) {
		return -1;
	}
	output = bin_open(output_path, 'w');
	if (!output) {
		return -2;
	}

	// Unpack input
	input_text = bin_file_to_string(input);
	print_serial(input_text);
	printf("Input: %s\n", input_text);
	huff_unpack_input(input_text, encoded_text, huffman_serial);
	printf("Encoded: %s\n", encoded_text);
	print_serial(huffman_serial);

	// Deserialize tree
	// Reconstruct text
	// Save decoded

	// free and close
	free(huffman_serial);
	free(decoded_text);
	Btree_free(huffman_tree);
	*input_size = bin_close(input);
	*output_size = bin_close(output);

	return 0;
}

// All malloc in huff_encode or huff_decode
// Reorganise functions
int main() {
	int code, input_size, output_size;

	code = huff_encode("./texts/hard.txt", "./texts/test_huff.bin", &input_size, &output_size);
	printf("code: %i\n", code);
	printf("input_size: %i\n", input_size);
	printf("output_size: %i\n", output_size);
	code = huff_decode("./texts/test_huff.bin", "./texts/test_huff_out.txt", &input_size, &output_size);
	printf("\ncode: %i\n", code);
	printf("input_size: %i\n", input_size);
	printf("output_size: %i\n", output_size);

	return 0;
}
