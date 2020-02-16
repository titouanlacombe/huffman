#include "binary_tree.h"

// alloc memory for the Binary tree and set tree pointers to NULL
Binary_tree *Btree_malloc() {
	Binary_tree *tree;
	tree = malloc(sizeof(Binary_tree));

	tree->left_child = NULL;
	tree->right_child = NULL;
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
	free(tree);
}

// Set a Binary tree values
Btree_create(Binary_tree *tree, Elt elt, int weight, Binary_tree *left_child, Binary_tree *right_child) {
	tree->elt = elt;
	tree->weight = weight;
	tree->left_child = left_child;
	tree->right_child = right_child;
}

// Create a leaf (no childs)
Binary_tree *Btree_create_leaf(Elt elt, int weight) {
	Binary_tree *tree;
	tree = Btree_malloc();
	Btree_create(tree, elt, weight, NULL, NULL);
	return tree;
}

// Set the path of the nodes in the tree from the root given in arg
Btree_set_paths(Binary_tree *root) {
	
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

// Serialize the tree and return a char*
char *Btree_serialize(Binary_tree *tree) {
	char c, *s1, *s2 ;
	
	if (tree->elt == '\0') {
		c = '\1';
		s1 = Btree_serialize(tree->left_child);
		s2 = Btree_serialize(tree->right_child);
		int n = strlen(s1) + strlen(s2) + 1;
		char s3[n];
		for (int i = 0; i < strlen(s1); i++) {
			s3[i] = s1[i];
		}
		for (int i = strlen(s1); i < strlen(s2); i++) {
			s3[i] = s2[i - strlen(s1)];
		}
		s3[n - 1] = c;
		return s3;
	} 
	else {
		char s3[1];
		s3[0] = tree->elt;
		return s3;
	}
}

// Deserialize the tree from the char* given in arg and return the tree aux
Binary_tree *Btree_deserialize_aux(char* s, int *n) {
	Binary_tree *tree;

	if (s[*n] != '\1') {
		tree = Btree_create_leaf(s[*n], 0);
		(*n)++;
		return tree;
	}
	else {
		tree = Btree_create_leaf('\0', 0);
		(*n)++;
		tree->left_child = Btree_deserialize_aux(s, n);
		tree->right_child = Btree_deserialize_aux(s, n);
		return tree;
	}
}

// Deserialize the tree from the char* given in arg and return the tree
Binary_tree *Btree_deserialize(char* serial) {
	int n = 0;
	Binary_tree *tree;
	tree = Btree_deserialize_aux(serial, &n);
	return tree;
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

int main() {

	return 0;
}