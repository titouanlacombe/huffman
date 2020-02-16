#include "binary_tree.h"

// alloc memory for the Binary tree and set tree path to NULL
Binary_tree *Btree_malloc() {
	Binary_tree *tree;
	tree = malloc(sizeof(Binary_tree));

	tree->path = NULL;

	return tree;
}

// Free a Binary tree
void Btree_free(Binary_tree *tree) {
	if (tree->left_child) {
		Btree_free(tree->left_child);
	}
	if (tree->right_child) {
		Btree_free(tree->right_child);
	}
	free(tree->path);
	free(tree);
}

// Set a Binary tree values
Binary_tree *Btree_create(Elt elt, int weight, Binary_tree *left_child, Binary_tree *right_child) {
	Binary_tree *tree;
	tree = Btree_malloc();

	tree->elt = elt;
	tree->weight = weight;
	tree->left_child = left_child;
	tree->right_child = right_child;

	return tree;
}

// Create a leaf (no childs)
Binary_tree *Btree_create_leaf(Elt elt, int weight) {
	Binary_tree *tree;
	tree = Btree_malloc();

	tree->elt = elt;
	tree->weight = weight;
	tree->left_child = NULL;
	tree->right_child = NULL;
	
	return tree;
}

// Create a node (no elt and weight)
Binary_tree *Btree_create_node(Binary_tree *left_child, Binary_tree *right_child) {
	Binary_tree *tree;
	tree = Btree_malloc();

	tree->elt = '\0';
	tree->weight = 0;
	tree->left_child = left_child;
	tree->right_child = right_child;
	
	return tree;
}

// Set the path of the nodes in the tree from the root given in arg aux
void Btree_set_paths_aux(Binary_tree *tree, char *path, int depth) {
	int i;

	tree->path = malloc(depth * sizeof(char) + 1);

	for (i = 0; i < depth; i++) {
		tree->path[i] = path[i];
	}
	tree->path[i] = '\0';
	
	if (tree->left_child) {
		path[depth] = '0';
		Btree_set_paths_aux(tree->left_child, path, depth + 1);
	}
	if (tree->right_child) {
		path[depth] = '1';
		Btree_set_paths_aux(tree->right_child, path, depth + 1);
	}
}

// Set the path of the nodes in the tree from the root given in arg
void Btree_set_paths(Binary_tree *root, int max_depth) {
	char path[max_depth + 1];
	Btree_set_paths_aux(root, path, 0);
}

// Return the Elt of the Binary tree
Elt Btree_get_elt(Binary_tree *tree) {
	Elt elt;
	elt = tree->elt;
	return elt;
}

// Return 1 if the Binary tree as no childs 0 else
int Btree_is_leaf(Binary_tree *tree) {
	return (!tree->left_child && !tree->right_child);
}

// Print a Binary tree aux
void Btree_print_aux(Binary_tree *tree) {
	if (tree->elt == '\0') {
		printf("Node: ");
		printf("lc(");
		Btree_print_aux(tree->left_child);
		printf("), rc(");
		Btree_print_aux(tree->right_child);
		printf(")");
	} 
	else {
		printf("Leaf: %c", tree->elt);
		printf(", %i", tree->weight);
		printf(", %s", tree->path);
	}
}
 
// Print a Binary tree
void Btree_print(Binary_tree *tree) {
	printf("Binary tree:\n");
	Btree_print_aux(tree);
	printf("\n");
}

/*
int main() {
	Binary_tree *tree1, *tree2, *tree3, *root;

	tree1 = Btree_create_leaf('a', 1);
	tree2 = Btree_create_leaf('b', 2);
	tree3 = Btree_create_leaf('c', 3);
	root = Btree_create_node(tree1, tree2);
	root = Btree_create_node(root, tree3);

	Btree_set_paths(root, 3);
	Btree_print(root);

	Btree_free(root);

	return 0;
}
*/