#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "test.h"
#include "bin_file.h"
#include "binary_tree.h"

int huff_encode(char *input_path, char *output_path, int *input_size, int *output_size);
void huff_decode();

// int huff_build_leaves(char *input_text, Binary_tree *leaves[]);
// Binary_tree *huff_build_tree(Binary_tree *leaves[]);

// char *huff_serialize_tree(Binary_tree *tree);
// Binary_tree *huff_deserialize_tree(char *serial);

// char *huff_tree_encode(char *input_text, Binary_tree *leaves[]);
// void huff_tree_decode();

// void huff_build_output(Bin_file *output, char *encoded_text, char *huffman_serial);