#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

#include "bst.c"

// function to turn a tree into an array
void tree2array(Node *start, Node *array[], int *index) {
	if (start == NULL) return;
	tree2array(start->left, array, index);
	array[*index] = start;
	(*index)++;
	tree2array(start->right, array, index);
}

// function for qsort (qsort is so cool)
// dont ask why the parameters are constant void lmao c is weird
int cmp_val(const void *coughing_baby, const void *atomic_bomb) {
	// ah yes let me have a pointer to a pointer but then derefence it. this is required. what are we doing
	Node *node1 = *(Node**)coughing_baby;
	Node *node2 = *(Node**)atomic_bomb;
	return node2->value - node1->value;
}

int main() {
	// file stuff, getting pointer to file
	printf("enter name of file you want word count for: ");
	char file_name[1000];
	scanf("%999s", file_name);
	FILE* fptr = fopen(file_name, "r");
	if (fptr == NULL) {
		printf("file doesnt exist\n");
		return 0;
	}
	
	printf("warning: due to me being stupid, the terminal is gonna print like a billion lines of text. dont worry its not broken it just kinda has to do that lmao");
	printf("\n\n");
	sleep(3);
	// tree to store word counts
	bst *word_counts = malloc(sizeof(bst));
	if (word_counts == NULL) {
		printf("memory allocation failed get more RAM loser\n");
		return 0;
	}
	init_bst(word_counts);
	
	// collect data: go through file, for each word either make a new node or update existing nodes count by 1
	char word[100];
	while (fscanf(fptr, "%99s", word) != EOF) {
		// remove punctuation and make everything lowercase
		int word_length = 0;
		for (int i = 0; word[i] != '\0'; i++) {
			if (isalpha((unsigned char)word[i])) {
				word[word_length] = tolower((unsigned char)word[i]);
				word_length++;
			}
		}
		word[word_length] = '\0';
		
		// if word is real (at least one letter), add to tree
		if (word_length > 0) {
			int word_count = lookup(word_counts, word);
			if (word_count >= 0) add(word_counts, word, word_count + 1, 0); // if node exists update value
			else { // if node doesnt exist make a new one
				char *wordcpy = malloc(strlen(word) + 1);
				strcpy(wordcpy, word);
				add(word_counts, wordcpy, 1, 1);
			}
		}
	}
	fclose(fptr);
	
	// remove common words from the tree
	char common_words[200][20] = {"a", "an", "the", "and", "or", "but", "if",
	"while", "because", "so", "im", "youre", "hes", "shes", "theyre", "weve",
	"than", "too", "very", "in", "on", "at", "by", "for", "with", "about",
	"against", "between", "into", "through", "during", "before", "after",
	"above", "below", "to", "from", "up", "down", "out", "over", "under",
	"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you",
	"your", "yours", "yourself", "yourselves", "he", "him", "his", "himself",
	"she", "her", "hers", "herself", "it", "its", "itself", "they", "them",
	"their", "theirs", "themselves", "what", "which", "who", "whom", "this",
	"that", "these", "those", "am", "is", "are", "was", "were", "be",
	"been", "being", "have", "has", "had", "having", "do", "does", "did",
	"doing", "will", "would", "shall", "should", "may", "might", "must",
	"can", "could", "all", "any", "both", "each", "few", "more", "most",
	"other", "some", "such", "no", "nor", "not", "only", "own", "same",
	"just", "now", "here", "there", "when", "where", "why", "how", "of"};
	for (int i = 0; i < 200; i++) {
		delete(word_counts, common_words[i]);
	}
	
	int n;
	printf("how many words would you like data for? ");
	scanf("%d", &n);
	if (n < 1) {
		printf("sorry you're not getting away with crashing my program :P\n");
		return 0;
	}
	if (n == 69) printf("nice.\n");
	if (n == 67) {
		while (1) {
			void* punishment = malloc(9999);
			sleep(1);
		}
	}
	// prints n most common words in descending order (most frequent words at top)
	int total_words = num_nodes(word_counts->root);
	Node** node_array = malloc(total_words * sizeof(Node*));
	int whydoesthisvariablehavetoexist = 0;
	tree2array(word_counts->root, node_array, &whydoesthisvariablehavetoexist);
	qsort(node_array, total_words, sizeof(Node*), cmp_val); // qsort is so cool
	
	int num2print = (total_words < n) ? total_words : n;
	for (int i = 0; i < num2print; i++) {
		printf("%s: appears %d times\n", node_array[i]->key, node_array[i]->value);
	}
	
	free(node_array);
	free_tree(word_counts);
	free(word_counts);
	return 1;
}
