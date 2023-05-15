#ifndef TREE_H
#define TREE_H
#include <stdbool.h>

// Node object in a binary tree.
typedef struct node {
    // left child
    struct node *left;

    // right child
    struct node *right;

    // Extra data at a node if needed
    void *data;
} node_t;

// A binary tree object
typedef struct tree
{   
    // Root node of the binary tree
    node_t *root;

    // The ith element of this array counts the number of nodes at the ith
    // level of the tree. The 0th level is NOT the tree root and is instead the 
    // level after (the children of the root tree). It is the responsibility of 
    // the progammer to update the values of this array - it is not internally 
    // handled by the tree object
    int *nodes_at_level;

    int depth;
} tree_t;

// Initiate an empty binary tree where the nodes_at_level array has length 'depth'.
// The 0th level is NOT the tree root and is instead the level after (the 
// children of the root tree). It is the responsibility of the progammer to 
// update the values of this array - it is not internally handled by the tree object
tree_t *new_tree(int depth);

// Initiate an empty tree node
node_t *new_node();

// Count the number of paths of length greater than or equal the given depth
int count_paths(tree_t *tree, int depth);

// Create a new tree with the common paths between tree1 and tree2.
// Assuming the 'nodes_at_level' array has the correct values for the input 
// trees, the output tree will also have the correct array values
tree_t *intersect_trees(tree_t *tree1, tree_t *tree2);

// Check if tree1 is a subset of tree2
bool subset(tree_t *tree1, tree_t *tree2);

#endif
