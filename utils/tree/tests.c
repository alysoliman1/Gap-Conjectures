#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

int main(){
    /*
    Test 1 for tree intersection

    tree 1:
             *
           /   \
          *     *
         / \   / \
        *   * *   *
       /   /   \
      *   *     *    

    tree 2:
             *
           /   \
          *     *
         / \   / 
        *   * *   
           /   \
          *     *    


    Intersection:
             
             *
           /   \
          *     *
         / \   / 
        *   * *   
           /   \
          *     *   

    */

    tree_t *tree1 = new_tree(4);
    node_t *root1 = tree1->root;
    root1->left = new_node();
    root1->right = new_node();
    root1->left->left = new_node();
    root1->left->right = new_node();
    root1->left->left->left = new_node();
    root1->left->right->left = new_node();
    root1->right->left = new_node();
    root1->right->right = new_node();
    root1->right->left->right = new_node();
    
    assert(tree1->depth == 4);
    assert(count_paths(tree1, 1) == 1);
    assert(count_paths(tree1, 2) == 2);
    assert(count_paths(tree1, 3) == 4);
    assert(count_paths(tree1, 4) == 3);

    tree_t *tree2 = new_tree(4);
    node_t *root2 = tree2->root;
    root2->left = new_node();
    root2->right = new_node();
    root2->left->left = new_node();
    root2->left->right = new_node();
    root2->left->right->left = new_node();
    root2->right->left = new_node();
    root2->right->left->right = new_node();

    tree_t *tree = intersect_trees(tree1, tree2);
    node_t *root = tree->root;

    assert(tree->depth == 4);
    assert(count_paths(tree, 3) == 3);
    assert(count_paths(tree, 4) == 2);

    assert(subset(tree, tree1));
    assert(subset(tree, tree2));
    assert(subset(tree2, tree));

    assert(tree->nodes_at_level[0] == 1);
    assert(tree->nodes_at_level[1] == 2);
    assert(tree->nodes_at_level[2] == 3);
    assert(tree->nodes_at_level[3] == 2);

    assert(root->right != NULL);
    assert(root->right->left != NULL);
    assert(root->right->left->left == NULL);
    assert(root->right->left->right != NULL);

    assert(root->left != NULL);
    assert(root->left->left != NULL);
    assert(root->left->left->left == NULL);
    assert(root->left->left->right == NULL);
    assert(root->left->right != NULL);
    assert(root->left->right->left != NULL);
    assert(root->left->right->right == NULL);

    return 0;
}
