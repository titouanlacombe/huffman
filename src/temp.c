
// Serialize the tree and return a char*
char *Btree_serialize(Binary_tree *tree) {

}

// Deserialize the tree from the char* given in arg and return the tree aux
Binary_tree *Btree_deserialize_aux(char* s, int *n) {

}

// Deserialize the tree from the char* given in arg and return the tree
Binary_tree *Btree_deserialize(char* serial) {
	int n = 0;
	Binary_tree *tree;
	tree = Btree_deserialize_aux(serial, &n);
	return tree;
}
