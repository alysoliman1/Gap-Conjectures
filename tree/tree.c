#include <stdlib.h>
#include "tree.h"

node_t *new_node() {
    node_t *node = malloc(sizeof(node_t));
    node->left  = NULL;
    node->right = NULL;
    return node;
}

tree_t *new_tree_with_root(node_t *root, int depth){
    tree_t *tree = malloc(sizeof(tree_t));
    tree->depth = depth;
    tree->root = root;
    tree->nodes_at_level = malloc(depth * sizeof(int));
    for (int n = 0; n < depth; n ++){
        tree->nodes_at_level[n] = 0;
    }
    return tree;
}

tree_t *new_tree(int depth){
    return new_tree_with_root(new_node(), depth);
}
 
int _count_paths(node_t *root, int depth){
    // An empty tree has zero paths
    if (root == NULL){
        return 0;
    }

    // The path containing the root node
    if (depth == 0){
        return 1;
    }
    
    // Recursive step
    int left_count = _count_paths(root->left, depth - 1);
    int right_count = _count_paths(root->right, depth - 1);
    return left_count + right_count;
}

int count_paths(tree_t *tree, int depth){
    return _count_paths(tree->root, depth);
}

node_t *_intersect_trees(node_t *root1, node_t *root2, int level, int *nodes_at_level) {
    // The intersection with an empty tree is empty
    if (root1 == NULL || root2 == NULL){
        return NULL;
    }

    // Otherwise if root1 and root2 exist then their intersection is a new node
    node_t *root = new_node();

    // Update the `nodes_at_level` array
    nodes_at_level[level] += 1;

    root->left = _intersect_trees(root1->left, root2->left, level + 1, nodes_at_level);
    root->right = _intersect_trees(root1->right, root2->right, level + 1, nodes_at_level);
    return root;
}

tree_t *intersect_trees(tree_t *tree1, tree_t *tree2) {
    tree_t *intersection = malloc(sizeof(tree_t));
    
    // Set the resulting tree depth to be min(depth of tree1, depth of tree2)
    intersection->depth = (tree1->depth < tree2->depth) ? tree1->depth : tree2->depth;

    // Given the tree length, initiate the 'nodes_at_level' array
    intersection->nodes_at_level = malloc(intersection->depth * sizeof(int));
    for (int i = 0; i < intersection->depth; i++){
        intersection->nodes_at_level[i] = 0;
    }

    // Update the tree with the correct node structure and `nodes_at_level` values
    intersection->root = _intersect_trees(tree1->root, tree2->root, 0, intersection->nodes_at_level);
    return intersection;
}

// Test if the tree at root1 is a subset of the tree at root2. We only care 
// about the node structure 
bool _subset(node_t *root1, node_t *root2){
    // The empty tree is the subset of any tree
    if (root1 == NULL){
        return true;
    }

    // The empty tree doesn't contain any non-empty trees
    if (root2 == NULL){
        return false;
    }

    // If root1's left subtree is not a subset of root2's left subtree
    bool left = _subset(root1->left, root2->left);
    if (!left) {
        return false;
    }

    // Check if root1's right subtree is a subset of root2's right subtree
    return _subset(root1->right, root2->right);
}

bool subset(tree_t *tree1, tree_t *tree2){
    return _subset(tree1->root, tree2->root);
}
