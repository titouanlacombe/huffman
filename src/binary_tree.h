#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

typedef char Elt;

typedef struct Binary_tree {
	Elt elt;           		// element stored by the node
	int weight;       	 	// node weight (usefull for the huffman algorithm)
	char *path;						// path from the root of the tree (0:left, 1:right)
	struct Binary_tree *left_child; 	// left child of the tree
	struct Binary_tree *right_child;	// right child of the tree
} Binary_tree;

Binary_tree *Btree_malloc();
void Btree_free(Binary_tree *tree);

Binary_tree *Btree_create(Elt elt, int weight, Binary_tree *left_child, Binary_tree *right_child);
Binary_tree *Btree_create_leaf(Elt elt, int weight);
Binary_tree *Btree_create_node(Binary_tree *left_child, Binary_tree *right_child);

Elt Btree_get_elt(Binary_tree *tree);
int Btree_is_leaf(Binary_tree *tree);
int Btree_get_leaves(Binary_tree *root, Binary_tree *leaves[]);
void Btree_set_paths(Binary_tree *root, int max_depth);
void Btree_print(Binary_tree *tree);
