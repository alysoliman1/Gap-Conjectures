#include "../utils/tree/tree.h"
#include "../utils/json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Check if a point on the circle is in the open interval (0.5 - xi, 0.5 + xi)
bool inrange(float point, float xi){
    return (point <= 0.5 + xi) && (point >= 0.5 - xi);
}

tree_t *circle_encodings(int N, long p, float xi){
    // Create an empty tree of depth N + 1 (the root of the tree is not used)
    tree_t *tree = new_tree(N + 1); 

    // Variables to be used later
    node_t *current_node, *next_node;
    bool move_right;
    float point;

    for (int i = 1; i < p; i ++){
        // alpha = i / p

        // Start at the root of the tree
        current_node = tree->root;

        for (int n = 0; n < N + 1; n ++){
            // Get the point n * alpha on the circle
            point = (float)((n * i) % p) / (float) p;

            // Check if the point is in the open interval (0.5 - xi, 0.5 + xi). 
            // If so then we move right down the tree, otherwise we move left.
            move_right = inrange(point, xi);
            if (move_right){
                next_node = current_node->right;
            } else {
                next_node = current_node->left;
            }

            // If this path has not been traversed before then create a new node
            if (next_node == NULL){
                next_node = new_node();
                if (move_right){
                    current_node->right = next_node;
                } else {
                    current_node->left = next_node;
                }
                
                // Update the number of nodes at the given level
                tree->nodes_at_level[n + 1] += 1;
            }
            current_node = next_node;
        }
    }
    
    return tree;
}

int main(){
    clock_t start_time, stop_time;
    double time_elapsed;

    // The integer p is a really large prime, the circle R/Z will be 
    // approximated by Z_p
    long p = 81677047; 

    // We're computing L(xi, k) for all 1 <= k < N
    // L(xi, k) is a lower bound on the number of sets in [k] = {0, ..., k - 1}
    // that are of the form A(alpha, (0.5 - xi, 0.5 + xi)) \cap [k] for some 
    // alpha. Elements in L(xi, k) are subsets of [k] and can be represented 
    // as paths in a binary tree of depth k
    int N = 1000;
    float xi = 0.125;
    start_time = clock();
    tree_t *tree = circle_encodings(N, p, xi);
    stop_time = clock();
    time_elapsed = (double) (stop_time - start_time) / CLOCKS_PER_SEC;
    printf("time elapsed: %f seconds\n", time_elapsed);    

    dump_array("lowerbounds.json", tree->nodes_at_level, N);
    return 0;
}