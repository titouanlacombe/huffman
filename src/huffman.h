#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "bin_file.h"
#include "binary_tree.h"

void huff_encode();
void huff_decode();

void huff_build_tree();
void huff_get_tree_leaves();

char *huff_serialize_tree(Binary_tree *tree);
Binary_tree *huff_deserialize_tree(char *serial);

void huff_tree_encode();
void huff_tree_decode();