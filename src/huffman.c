#include "huffman.h"

#define SERIAL_NODE 1
#define SERIAL_LEAF 2
#define SERIAL_END 3
#define MAX_SERIAL_SIZE 3*UCHAR_MAX+1

// For each unique char in input_text create a leaf in leaf with 
// elt as the unique char and weight as it's number of aparition in input_text
int build_tree_leaves(char *input_text, Binary_tree *leaves[]) {
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
void sort_leaves(Binary_tree *leaves[], int nb_leaves) {
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
void insert_in_leaves(Binary_tree *leaves[], Binary_tree *tree, int nb_leaves) {
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
Binary_tree *build_huff_tree(Binary_tree *leaves[], int nb_leaves) {
	Binary_tree *tree1, *tree2, *tree3, *leaves_copy[nb_leaves];

	sort_leaves(leaves, nb_leaves);
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
		insert_in_leaves(leaves_copy, tree3, nb_leaves);
		nb_leaves--;
	}
	return leaves_copy[0];
}

// return the length of the encoded input_text
int calc_encoded_size(char *input_text, Binary_tree *leaves[], int nb_leaves) {
	int sum;

	sum = 0;
	for (int i = 0; i < nb_leaves; i++) {
		sum += leaves[i]->weight * strlen(leaves[i]->path);
	}
	return sum;
}

// Find a leaf whose elt is elt
Binary_tree *find_elt(char elt, Binary_tree *leaves[], int nb_leaves) {
	for (int i = 0; i < nb_leaves; i++) {
		if (leaves[i]->elt == elt) {
			return leaves[i];
		}
	}
}

// append dest by str with start the position in dest to append to
void strappend(char *dest, char *str, int start) {
	for (int i = 0; i < strlen(str); i++) {
		dest[i + start] = str[i];
	}
}

// Encode the input_text with the huffman compression using the leaves and their paths
int encode_string(char *input_text, char *encoded_text, Binary_tree *leaves[], int nb_leaves) {
	Binary_tree *leaf;
	int encoded_size, i;

	encoded_size = 0;
	i = 0;
	while (i < strlen(input_text)) {
		leaf = find_elt(input_text[i], leaves, nb_leaves);
		strappend(encoded_text, leaf->path, encoded_size);
		encoded_size += strlen(leaf->path);
		i++;
	}
	input_text[i] = '\0';

	return encoded_size;
}

// decode the encoded text using the huffman tree aux
char decode_string_aux(char *encoded_text, Binary_tree *branch, int *j) {
	if (Btree_is_leaf(branch)) {
		return branch->elt;
	}
	else {
		if (encoded_text[*j] == '0') {
			(*j)++;
			return decode_string_aux(encoded_text, branch->left_child, j);
		}
		else if (encoded_text[*j] == '1') {
			(*j)++;
			return decode_string_aux(encoded_text, branch->right_child, j);
		}
		else {
			return '\0';
		}
	}
}

// decode the encoded text using the huffman tree
char *decode_string(char *encoded_text, Binary_tree *huffman_tree, int encoded_size) {
	char c, *decoded_text;
	int i, j;

	decoded_text = malloc(strlen(encoded_text));
	i = 0;
	j = 0;
	while (j < encoded_size) {
		c = decode_string_aux(encoded_text, huffman_tree, &j);
		decoded_text[i] = c;
		i++;
	}
	decoded_text[i] = '\0';
	
	return decoded_text;
}

// Serialize the tree and return a char* aux
void serialize_tree_aux(Binary_tree *tree, char *serial_tree, int *serial_tree_i) {
	if (Btree_is_leaf(tree)) {
		serial_tree[*serial_tree_i] = SERIAL_LEAF;
		(*serial_tree_i)++;
		serial_tree[*serial_tree_i] = tree->elt;
		(*serial_tree_i)++;
	}
	else {
		serial_tree[*serial_tree_i] = SERIAL_NODE;
		(*serial_tree_i)++;
		serialize_tree_aux(tree->left_child, serial_tree, serial_tree_i);
		serialize_tree_aux(tree->right_child, serial_tree, serial_tree_i);
	}
}

// Serialize the tree and return a char*
// recursive
// node->0
// leaf->1 + 'elt'
int serialize_tree(Binary_tree *tree, char *serial_tree) {
	int i = 0;
	serialize_tree_aux(tree, serial_tree, &i);
	serial_tree[i] = SERIAL_END;
	i++;
	return i;
}

// Deserialize a binary tree aux
Binary_tree *deserialize_tree_aux(char *serial, int *i) {
	Binary_tree *tree1, *tree2;

	if (serial[*i] == SERIAL_LEAF) {
		(*i)++;
		tree1 = Btree_create_leaf(serial[*i], 0);
		(*i)++;
		return tree1;
	}
	else if (serial[*i] == SERIAL_NODE) {
		(*i)++;
		tree1 = deserialize_tree_aux(serial, i);
		tree2 = deserialize_tree_aux(serial, i);
		tree2 = Btree_create_node(tree1, tree2);
		return tree2;
	}
}

// Deserialize a binary tree
Binary_tree *deserialize_tree(char *serial) {
	int i = 0;
	return deserialize_tree_aux(serial, &i);
}

// Write in the output_file file the encoded_text and the huffman_serial
void pack_output(Bin_file *output_file, char *encoded_text, char *huffman_serial, int encoded_size) {
	int i;

	putw(encoded_size, output_file->file);
	
	i = 0;
	while (huffman_serial[i] != SERIAL_END) {
		bin_write_byte(output_file, huffman_serial[i]);
		i++;
	}
	bin_write_byte(output_file, SERIAL_END);

	i = 0;
	while (i < encoded_size) {
		bin_write_bit(output_file, encoded_text[i]);
		i++;
	}
}

// Unpack the differents informations from the input_file
void unpack_input(Bin_file *input_file, char *huffman_serial, char *encoded_text, int encoded_size) {
	int i;
	char c;

	i = 0;
	while ((c = bin_read_byte(input_file)) != SERIAL_END) {
		huffman_serial[i] = c;
		i++;
	}
	huffman_serial[i] = SERIAL_END;
	
	i = 0;
	while (i < encoded_size) {
		c = bin_read_bit(input_file);
		encoded_text[i] = c;
		i++;
	}
	encoded_text[i] = '\0';
}

// Save the decoded to the output_file
void save_str(Bin_file *output_file, char *decoded_text) {
	int i;

	i = 0;
	while (decoded_text[i] != '\0') {
		bin_write_byte(output_file, decoded_text[i]);
		i++;
	}
}

// Print a serialized tree
void print_tree_serial(char *huffman_serial) {
	int i = 0;

	printf("Serial: ");
	while (huffman_serial[i] != SERIAL_END) {
		if (huffman_serial[i] == SERIAL_NODE) {
			printf("node|");
			i += 1;
		} else if (huffman_serial[i] == SERIAL_LEAF) {
			printf("leaf|");
			printf("%c|", huffman_serial[i+1]);
			i += 2;
		}
	}
	printf("end\n");
}

// Encode the input_path file and puts the results in the output_file file
// Errors codes:
// -1: Unable to open input_path
// -2: Unable to open output_path
int huff_encode(char *input_path, char *output_path, int *input_size, int *output_size) {
	int nb_leaves, encoded_size;
	char *input_text, *huffman_serial, *encoded_text;
	Bin_file *input_file, *output_file;
	Binary_tree *leaves[UCHAR_MAX + 1], *huffman_tree;

	input_file = bin_open(input_path, 'r');
	if (!input_file) {
		return -1;
	}
	output_file = bin_open(output_path, 'w');
	if (!output_file) {
		return -2;
	}

	// Get input_file
	input_text = bin_file_to_string(input_file);

	// Construct huffman tree
	nb_leaves = build_tree_leaves(input_text, leaves);	
	huffman_tree = build_huff_tree(leaves, nb_leaves);
	
	// Serialize tree
	Binary_tree *nodes[nb_leaves];
	huffman_serial = malloc(MAX_SERIAL_SIZE*sizeof(char));
	serialize_tree(huffman_tree, huffman_serial);
	
	// Encode text
	Btree_set_paths(huffman_tree, nb_leaves + 1);
	encoded_text = malloc(calc_encoded_size(input_text, leaves, nb_leaves)*sizeof(char));
	encoded_size = encode_string(input_text, encoded_text, leaves, nb_leaves);

	// Construct output_file
	pack_output(output_file, encoded_text, huffman_serial, encoded_size);

	// free and close
	free(input_text);
	free(huffman_serial);
	free(encoded_text);
	Btree_free(huffman_tree);
	*input_size = bin_close(input_file);
	*output_size = bin_close(output_file);

	return 0;
}

// Decode the input_path file and puts the results in the output_file file
// Errors codes:
// -1: Unable to open input_path
// -2: Unable to open output_path
int huff_decode(char *input_path, char *output_path, int *input_size, int *output_size) {
	int encoded_size;
	char *decoded_text, *encoded_text, *huffman_serial;
	Bin_file *input_file, *output_file;
	Binary_tree *huffman_tree;

	input_file = bin_open(input_path, 'r');
	if (!input_file) {
		return -1;
	}
	output_file = bin_open(output_path, 'w');
	if (!output_file) {
		return -2;
	}

	// Unpack input_file
	encoded_size = getw(input_file->file);
	
	huffman_serial = malloc(MAX_SERIAL_SIZE*sizeof(char));
	encoded_text = malloc(encoded_size*sizeof(char));
	unpack_input(input_file, huffman_serial, encoded_text, encoded_size);

	// Deserialize tree
	huffman_tree = deserialize_tree(huffman_serial);

	// Reconstruct text
	decoded_text = decode_string(encoded_text, huffman_tree, encoded_size);

	// Save decoded
	save_str(output_file, decoded_text);

	// free and close
	free(encoded_text);
	free(huffman_serial);
	free(decoded_text);
	Btree_free(huffman_tree);
	*input_size = bin_close(input_file);
	*output_size = bin_close(output_file);

	return 0;
}

/*
int main() {
	int code, input_size, output_size;

	printf("--------------------Encode--------------------\n");
	code = huff_encode("./texts/medium.txt", "./texts/test_huff_out.bin", &input_size, &output_size);
	printf("input_size: %i\n", input_size);
	printf("output_size: %i\n", output_size);
	printf("--------------------Decode--------------------\n");
	code = huff_decode("./texts/test_huff_out.bin", "./texts/test_huff_out.txt", &input_size, &output_size);
	printf("input_size: %i\n", input_size);
	printf("output_size: %i\n", output_size);

	return 0;
}*/
