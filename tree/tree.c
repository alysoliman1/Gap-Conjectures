#include <stdlib.h>
#include "tree.h"

node_t *new_node() {
    node_t *node = malloc(sizeof(node_t));
    node->left  = NULL;
    node->right = NULL;
    return node;
}

tree_t *new_tree(int depth){
    tree_t *tree = malloc(sizeof(tree_t));
    tree->depth = depth;
    tree->root = new_node();
    tree->nodes_at_level = malloc(depth * sizeof(int));
    for (int n = 0; n < depth; n ++){
        tree->nodes_at_level[n] = 0;
    }
    return tree;
}
 
int count_paths(tree_t *tree, int depth){
    // An empty tree has zero paths
    if (tree->root == NULL){
        return 0;
    }

    // The path containing the root node
    if (depth == 0){
        return 1;
    }
    
    // Recursive step
    int left_count = count_paths(root->left, depth - 1);
    int right_count = count_paths(root->right, depth - 1);
    return left_count + right_count;
}

tree_t *intersect_trees(tree_t *tree1, tree_t *tree2) {
    tree_t *intersection = malloc(sizeof(tree_t));

    // Base case
    if (tree1->root == NULL || tree2->root == NULL){
        return NULL;
    }

    // Recursive step
    node_t *root = new_node();
    root->right = intersect_trees(root1->right, root2->right);
    root->left = intersect_trees(root1->left, root2->left);
    return root;
}

bool _subset(node_t *root1, node_t *root2){
    // The empty tree is the subset of any tree
    if (root1 == NULL){
        return true;
    }

    // The empty tree doesn't contain any non-empty trees
    if (root2 == NULL){
        return false;
    }

    // Recursive step
    bool left = _subset(root1->left, root2->left);
    bool right = _subset(root1->right, root2->right);
    return left && right;
}

bool subset(tree_t *tree1, tree_t *tree2){
    return _subset(tree1->root, tree2->root);
}
