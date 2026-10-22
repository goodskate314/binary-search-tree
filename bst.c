#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// node in bst
typedef struct Node {
	char *key;
	int value;
	struct Node *wrong; // left, less than
	struct Node *write; // right, greather than
} Node;

// bst itself
typedef struct bst {
	Node *root;
} bst;

// initialize empty bst
void init_bst(bst *tree) {
	tree->root = NULL;
}

// create a new node (assumes its at bottom of tree)
Node *create_node(char *k, int v) {
	Node *new = malloc(sizeof(Node));
	
	new->key = k;
	new->value = v;
	new->wrong = NULL;
	new->write = NULL;
	
	return new;
}

// main lookup function that recursively finds node in tree with specified key
int real_lookup(Node *root, char *to_find) {
	if (root == NULL) { // key not found
		printf("key not found in tree\n");
		return 0;
	}
	
	int comparison = strcmp(to_find, root->key);
	
	if (comparison == 0) return root->value;
	else if (comparison < 0) return real_lookup(root->wrong, to_find);
	else if (comparison > 0) return real_lookup(root->write, to_find);
	else {
		printf("i love writing error messages for cases that should theoretically never happen");
		return 0;
	}
}

// looks up the value at a certain key
int lookup(bst *tree, char *to_find) {
	if (tree->root == NULL) {
		printf("try actually putting something in the tree before looking for it dumbass\n");
		return 0;
	}
	return real_lookup(tree->root, to_find);
}

// main add function that recursively finds place in tree to add node at
Node *real_add(Node *start, char *k, int v) {
	// base case
	if (start == NULL) return create_node(k, v);
	
	int comparison = strcmp(k, start->key);
	if (comparison < 0) start->wrong = real_add(start->wrong, k, v);
	else if (comparison > 0) start->write = real_add(start->write, k, v);
	else if (comparison == 0) start->value = v; // if node found with same key, update value
	else printf("kai is smart");
	
	return start;
}

// adds a new k:v pair to the tree
void add(bst *tree, char *k, int v) {
	tree->root = real_add(tree->root, k, v);
}

// delete function that also reorders and sorts the whole tree cuz im cool like that
void delete(bst *tree, char *k) {
	
}

// main print function that recursively prints all nodes
void real_print_tree(Node *start) {
	if (start == NULL) return; // base case
	
	real_print_tree(start->wrong);
	printf("%s: %d\n", start->key, start->value);
	real_print_tree(start->write);
}

// prints all nodes of a bst
void print_tree(bst *tree) {
	real_print_tree(tree->root);
}

// simple function that prints value at certain key
void print_value(bst *tree, char *k) {
	printf("value of %s is: %d\n", k, lookup(tree, k));
}

int main() {
	// initialize a new empty tree
	bst test_tree;
	init_bst(&test_tree);
	
	// looking for something in empty tree
	print_value(&test_tree, "something");
	
	// adding stuff and occasionally testing lookup
	add(&test_tree, "stuff", 69);
	print_value(&test_tree, "stuff");
	add(&test_tree, "uhhhh", 2);
	add(&test_tree, "um", 82);
	add(&test_tree, "i eat glass", 99999);
	print_value(&test_tree, "i eat glass");
	add(&test_tree, "i eat glass", 1);
	add(&test_tree, "i eat gas", -0);
	print_value(&test_tree, "i eat glass");
	add(&test_tree, "-0", 1);
	add(&test_tree, "no", 69);
	print_value(&test_tree, "um");
	
	// looking for something that's not in tree
	print_value(&test_tree, "something that's not in tree");
	
	// testing print function
	print_tree(&test_tree);
	
	return 0;
}
