#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

// node in bst
typedef struct Node {
	char *key;
	int value;
	struct Node *left; // less than
	struct Node *right; // greather than
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
Node* create_node(char *k, int v) {
	Node *new = malloc(sizeof(Node));
	if (new == NULL) return NULL;
	
	new->key = k;
	new->value = v;
	new->left = NULL;
	new->right = NULL;
	
	return new;
}

// main lookup function that recursively finds node in tree with specified key
int real_lookup(Node *start, char *to_find) {
	if (start == NULL) {
		printf("key not found in tree\n");
		return -999999;
	}
	
	int comparison = strcmp(to_find, start->key);
	
	if (comparison == 0) return start->value;
	else if (comparison < 0) return real_lookup(start->left, to_find);
	else if (comparison > 0) return real_lookup(start->right, to_find);
	else {
		printf("i love writing error messages for cases that should theoretically never happen");
		return -999999;
	}
}

// looks up the value at a certain key
int lookup(bst *tree, char *to_find) {
	if (tree == NULL || tree->root == NULL) {
		printf("try actually putting something in the tree before looking for it dumbass\n");
		return -999999;
	}
	return real_lookup(tree->root, to_find);
}

// lookup but returns the whole node (used for delete)
Node* find_node(Node *start, char *to_find) {
	if (start == NULL) return NULL;
	
	int comparison = strcmp(to_find, start->key);
	
	if (comparison == 0) return start;
	else if (comparison < 0) return find_node(start->left, to_find);
	else if (comparison > 0) return find_node(start->right, to_find);
	else return NULL; // go away stupid warning
}

// function that counts the number of nodes past (and including) a certain node
int num_nodes(Node *start) {
	if (start == NULL) return 0;
	
	return num_nodes(start->left) + num_nodes(start->right) + 1;
}

// function that stores all nodes of a tree in alphabetical order
void store_in_order(Node *start, Node **nodes, int *index) {
	if (start == NULL) return;
	
	store_in_order(start->left, nodes, index);
	nodes[(*index)++] = start;
	store_in_order(start->right, nodes, index);
}

// function that (attempts to) build a balanced tree using an array of nodes
Node* build_balanced(Node **nodes, int start, int end) {
	if (start > end || nodes == NULL) return NULL;
	
	int mid = start + (end - start) / 2;
	Node *begin = nodes[mid];
	
	begin->left = build_balanced(nodes, start, mid - 1);
	begin->right = build_balanced(nodes, mid + 1, end);
	
	return begin;
}

// function that (attempts to) rebalance the tree to make it symmetrical
void rebalance(bst *tree) {
	if (tree == NULL || tree->root == NULL) return;
	
	int n = num_nodes(tree->root);
	Node **nodes = malloc(n * sizeof(Node *));
	
	if (nodes == NULL) { // if memory allocation fails
		printf("memory allocation failed while trying to rebalance tree\n");
		return;
	}
	
	int index = 0;
	store_in_order(tree->root, nodes, &index);
	
	tree->root = build_balanced(nodes, 0, n - 1);
	
	free(nodes);
}

// main find max depth function that works recursively
int real_max_depth(Node *start) {
	if (start == NULL) return 0;
	
	int left_depth = real_max_depth(start->left);
	int right_depth = real_max_depth(start->right);
	
	if (left_depth > right_depth) {
		return left_depth + 1;
	}
	else {
		return right_depth + 1;
	}
}

// function that takes a tree and returns the maximum depth of it (longest 'root -> end' branch)
int max_depth(bst *tree) {
	if (tree == NULL || tree->root == NULL) return 0;
	return real_max_depth(tree->root);
}

// how many goddamn helper functions does this stupid delete function need istg
Node* find_parent(Node *start, char* k) {
	Node *parent = NULL;
	Node *current = start;
	
	while (current != NULL && strcmp(current->key, k) != 0) {
		parent = current;
		int comparison = strcmp(k, current->key);
		if (comparison < 0) current = current->left;
		else if (comparison > 0) current = current->right;
		else printf("you weren't supposed to enter the while loop wtf");
	}
	
	if (current == NULL) return NULL;
	return parent;
}

// main add function that recursively finds place in tree to add node at
Node* real_add(Node *start, char *k, int v) {
	if (start == NULL) return create_node(k, v);
	
	int comparison = strcmp(k, start->key);
	if (comparison < 0) start->left = real_add(start->left, k, v);
	else if (comparison > 0) start->right = real_add(start->right, k, v);
	else if (comparison == 0) start->value = v; // if node found with same key, update value
	else printf("kai is smart");
	
	return start;
}

// adds a new k:v pair to the tree
void add(bst *tree, char *k, int v, int check_balance) {
	if (tree == NULL) return;
	if (tree->root == NULL) goto get_some_nodes_in_this_tree_right_now;
	
	// rebalance tree if its very inefficient (and if user asked to)
	if (check_balance == 1) {
		int n = num_nodes(tree->root);
		int depth = max_depth(tree);
		int min_depth = ceil(log2(n));
		if (n > 2 * min_depth) {
			printf("rebalancing tree... may take a while if tree is big\n");
			rebalance(tree);
			printf("done!\n");
		}
	}
	
	get_some_nodes_in_this_tree_right_now:
	;
	tree->root = real_add(tree->root, k, v);
}

// main delete function that... wait, this one isn't actually recursive? surely it's not that complicated then... right? right?? right?!?!?!?
void real_delete(Node *del, Node *parent) {
	if (del == NULL) {
		printf("imagine hating someone so much you try to kill them after they're already dead\n");
		return;
	}
	
	// case 0: deleting the root if the root has 2 children (skips to case 3)
	if (parent == NULL) goto case3;
	
	int dir; // 1 if node is a left child of parent, 0 if node is a right child of parent
	if (parent->left == del) dir = 1;
	else if (parent->right == del) dir = 0;
	else {
		printf("you fucked up\n");
		return;
	}
	
	// case 1: child has no children, kill the child and erase parent's memory of its child (just like in real life!)
	if (del->left == NULL && del->right == NULL) {
		dir ? (parent->left = NULL) : (parent->right = NULL);
		free(del);
		return;
	}
	
	// case 2: child has one child, kill original child and gaslight parent into thinking its grandchild is its child (just like in real life?)
	if (del->left == NULL) {
		dir ? (parent->left = del->right) : (parent->right = del->right);
		free(del);
		return;
	}
	else if (del->right == NULL) {
		dir ? (parent->left = del->left) : (parent->right = del->left);
		free(del);
		return;
	}
	
	// case 3: 2 children, swap positions of child and its least valuable child thats still greater than original child and then kill original child (guys i don't think this analogy works very well)
	case3:
	; // i love c
	
	Node *heir = del->right;
	if (heir == NULL) printf("???????????\n");
	Node *heirs_parent = del;
	while (heir->left != NULL) {
		heirs_parent = heir;
		heir = heir->left;
	}
	del->key = heir->key;
	del->value = heir->value;
	(heirs_parent == del) ? (heirs_parent->right = heir->right) : (heirs_parent->left = heir->right);
	free(heir);
}

// function that finds a node with a certain key and deletes it from the tree
void delete(bst *tree, char *k) {
	if (tree == NULL || tree->root == NULL) return;
	Node *to_delete = find_node(tree->root, k);
	if (to_delete == NULL) {
		printf("no node with that key found\n");
		return;
	}
	Node *p = find_parent(tree->root, k);
	
	// special case for deleting root if root doesn't have two children
	if (to_delete == tree->root && (to_delete->left == NULL || to_delete->right == NULL)) {
		if (to_delete->left == NULL && to_delete->right == NULL) tree->root = NULL; // goodbye tree
		else if (to_delete->left == NULL) tree->root = to_delete->right;
		else if (to_delete->right == NULL) tree->root = to_delete->left;
		free(to_delete);
		return;
	}
	
	else real_delete(to_delete, p);
}

// main print function that recursively prints all nodes
void real_print_tree(Node *start) {
	if (start == NULL) return;
	
	real_print_tree(start->left);
	printf("%s: %d\n", start->key, start->value);
	real_print_tree(start->right);
}

// prints all nodes of a bst
void print_tree(bst *tree) {
	if (tree == NULL || tree->root == NULL) return;
	printf("root of tree is: '%s'\n", tree->root->key);
	real_print_tree(tree->root);
}

// simple function that prints value at certain key
void print_value(bst *tree, char *k) {
	if (tree == NULL || tree->root == NULL) return;
	printf("value of %s is: %d\n", k, lookup(tree, k));
}

// ok fineeeeee ill fix the memory leak if you insist
void real_free_tree(Node *start) {
	if (start == NULL) return;
	
	real_free_tree(start->left);
	real_free_tree(start->right);
	free(start);
}

void free_tree(bst *tree) {
	if (tree == NULL || tree->root == NULL) return; // are you getting tired of seeing this exact if statement yet
	real_free_tree(tree->root);
	tree->root = NULL;
}
