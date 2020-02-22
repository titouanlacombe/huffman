#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "bin_file.h"
#include "binary_tree.h"

int huff_encode(char *input_path, char *output_path, int *input_size, int *output_size);
int huff_decode(char *input_path, char *output_path, int *input_size, int *output_size);