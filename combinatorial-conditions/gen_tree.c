#include "gen_tree.h"
#include <stdlib.h>

void _tree_from_context(child_context get_context, node_t *node, int *nodes_at_level, int level, int depth){
    nodes_at_level[level] += 1;

    if (level == depth - 1){
        return;
    }
    
    void *left_context = get_context(node->context, LEFT);
    void *right_context = get_context(node->context, RIGHT);

    if (left_context){
        node->left = new_node();
        node->left->context = left_context;
        _tree_from_context(get_context, node->left, nodes_at_level, level + 1, depth);
    }

    if (right_context){
        node->right = new_node();
        node->right->context = right_context;
        _tree_from_context(get_context, node->right, nodes_at_level, level + 1, depth);
    }
}

tree_t *tree_from_context(child_context get_context, void *root_context, int depth){
    tree_t *tree = new_tree(depth);
    tree->root->context = root_context;
    tree->nodes_at_level = malloc(depth * sizeof(int));
    for (int i = 0; i < depth; i++){
        tree->nodes_at_level[i] = 0;
    }
    _tree_from_context(get_context, tree->root, tree->nodes_at_level, 0, depth);
    return tree;
}