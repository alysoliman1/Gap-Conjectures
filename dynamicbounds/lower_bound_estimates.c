#include "../tree/tree.h"
#include "../json/json.h"
#include <stdio.h>
#include <stdlib.h>

// Note
// Read the json and tree modules before reading this code

// Check if a point on the circle is in the open interval (0.5 - xi, 0.5 + xi)
bool inrange(float point, float xi){
    return (point <= 0.5 + xi) && (point >= 0.5 - xi);
}

tree_t *circle_encodings(int N, long p, float xi){
    // Create an empty tree
    tree_t *tree = new_tree(N); 

    // Variables to be used later
    node_t *current;
    node_t *next;
    float point;
    bool right;

    for (int i = 1; i < p; i ++){
        // alpha = i / p

        // We will go back to the root of the tree
        current = tree->root;

        for (int n = 0; n < N; n ++){
            // Get the point {n * alpha} on the circle
            point = (float)((n * i) % p) / (float) p;

            // Check if the point is in the open interval
            // (0.5 - xi, 0.5 + xi). If it is then we move
            // right down the tree, otherwise we move left
            right = inrange(point, xi);
            
            if (right){
                next = current->right;
            } else {
                next = current->left;
            }

            // If this path has not been traversed before then create a new node
            if (next == NULL){
                tree->nodes_at_level[n] += 1;
                next = new_node();
                if (right){
                    current->right = next;
                } else {
                    current->left = next;
                }
            }

            current = next;
        }
    }
    
    return tree;
}

int main(){
    // p is a really large prime, the circle R/Z will be approximated by Z_p
    long p = 39916801;

    // We're computing L(xi, k) for all 1 <= k < N
    // L(xi, k) is a lower bound on the number of k-sets of the form A(alpha, (0.5 - xi, 0.5 + xi))
    // for some xi
    int N = 200;
    float xi = 0.1;



    dump_array("lowerbounds.json", bounds, N);
    return 0;
}