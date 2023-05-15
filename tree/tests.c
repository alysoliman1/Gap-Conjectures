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

    node_t *tree1 = new_node();
    tree1->left = new_node();
    tree1->right = new_node();
    tree1->left->left = new_node();
    tree1->left->right = new_node();
    tree1->left->left->left = new_node();
    tree1->left->right->left = new_node();
    tree1->right->left = new_node();
    tree1->right->right = new_node();
    tree1->right->left->right = new_node();

    node_t *tree2 = new_node();
    tree2->left = new_node();
    tree2->right = new_node();
    tree2->left->left = new_node();
    tree2->left->right = new_node();
    tree2->left->right->left = new_node();
    tree2->right->left = new_node();
    tree2->right->left->right = new_node();

    node_t *result = intersect_trees(tree1, tree2);

    assert(count_paths(result, 3) == 2);
    assert(subset(result, tree1));
    assert(subset(result, tree2));
    assert(subset(tree2, result));

    assert(result->left != NULL);
    assert(result->right != NULL);
    assert(result->left->left != NULL);
    assert(result->left->left->left == NULL);
    assert(result->left->left->right == NULL);
    assert(result->left->right != NULL);
    assert(result->left->right->left != NULL);
    assert(result->left->right->right == NULL);
    assert(result->right->left != NULL);
    assert(result->right->left->left == NULL);
    assert(result->right->left->right != NULL);

    return 0;
}
