#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

#include "bst.c"

int main() {
	// initialize a new empty tree
        bst test_tree;
        init_bst(&test_tree);

        // looking for something in empty tree
        print_value(&test_tree, "something");

        // adding stuff and occasionally testing lookup
        add(&test_tree, "50", 50, 0);
        add(&test_tree, "0", 0, 0);
        print_value(&test_tree, "0");
        add(&test_tree, "1", 1, 0);
        add(&test_tree, "2", 5, 0);
        add(&test_tree, "3", 3, 0);
        print_value(&test_tree, "2");
        add(&test_tree, "2", 2, 0);
        add(&test_tree, "26", 26, 0);
        print_value(&test_tree, "2");
        add(&test_tree, "75", 75, 0);
        add(&test_tree, "56", 56, 0);
        print_value(&test_tree, "1");
        add(&test_tree, "19", 19, 0);
        add(&test_tree, "93", 93, 0);
        add(&test_tree, "66", 66, 0);
        add(&test_tree, "29", 29, 0);
        add(&test_tree, "49", 49, 0);
        add(&test_tree, "30", 30, 0);
        add(&test_tree, "100", 100, 0);
        add(&test_tree, "69", 69, 0);
        add(&test_tree, "99", 99, 0);
        add(&test_tree, "85", 85, 0);

        // looking for something that's not in tree
        print_value(&test_tree, "something that's not in tree");

        // testing print function
        printf("printing all nodes of tree: \n");
        print_tree(&test_tree);
        printf("done printing all nodes of tree\n");

        // counting amount of nodes in tree
        printf("number of nodes in tree: %d\n", num_nodes(test_tree.root));

        // testing delete function with (i think) all possible cases
        delete(&test_tree, "85"); // leaf node
        delete(&test_tree, "93"); // one child
        delete(&test_tree, "3"); // two children
        delete(&test_tree, "50"); // root
        delete(&test_tree, "hi"); // key thats not in tree
        printf("number of nodes in tree (should be 4 less than before): %d\n", num_nodes(test_tree.root));

        // testing rebalance function
        printf("current max depth: %d\n", max_depth(&test_tree));
        rebalance(&test_tree);
        printf("max depth after rebalance: %d\n", max_depth(&test_tree));

        printf("printing all nodes of tree (again): \n");
        print_tree(&test_tree);
        printf("done printing all nodes of tree\n");

        free_tree(&test_tree);
	return 0;
}
