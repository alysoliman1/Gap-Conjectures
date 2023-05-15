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

    node_t *root1 = new_node();
    tree_t *tree1 = new_tree_with_root(root1, 4);
    root1->left = new_node();
    root1->right = new_node();
    root1->left->left = new_node();
    root1->left->right = new_node();
    root1->left->left->left = new_node();
    root1->left->right->left = new_node();
    root1->right->left = new_node();
    root1->right->right = new_node();
    root1->right->left->right = new_node();
    

    node_t *root2 = new_node();
    tree_t *tree2 = new_tree_with_root(root2, 4);
    root1->left = new_node();
    root2->left = new_node();
    root2->right = new_node();
    root2->left->left = new_node();
    root2->left->right = new_node();
    root2->left->right->left = new_node();
    root2->right->left = new_node();
    root2->right->left->right = new_node();

    tree_t *tree = intersect_trees(tree1, tree2);
    node_t *root = tree->root;

    // assert(count_paths(tree, 3) == 2);
    assert(subset(tree, tree1));
    assert(subset(tree, tree2));
    assert(subset(tree2, tree));

    assert(root->left != NULL);
    assert(root->right != NULL);
    assert(root->left->left != NULL);
    assert(root->left->left->left == NULL);
    assert(root->left->left->right == NULL);
    assert(root->left->right != NULL);
    assert(root->left->right->left != NULL);
    assert(root->left->right->right == NULL);
    assert(root->right->left != NULL);
    assert(root->right->left->left == NULL);
    assert(root->right->left->right != NULL);

    return 0;
}
