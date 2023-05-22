#ifndef GEN_TREE_H
#define GEN_TREE_H
#include "../tree/tree.h"

// A context is a finite collection of data associated with every node in a binary
// tree. The idea is that, given the context of a node, we are able to decide if
// it has a left or right child, and, if so, produce the contexts for those children.
// Thus, given the context of the root node, we are able to generate an arbitrarly 
// large tree. 

// See contexts.h for examples of contexts

// A function of type `child_context` accepts the context for a node and a direction
// (either LEFT or RIGHT). If the node has a child in that direction then that child's
// context is returned. Otherwise if, according to the node's context, the node can't
// have a child in that direction then NULL is returned. Each context type has a
// 'child_context' function associated with it
typedef void *child_context(void *context, int direction); 

// Given the context of the root node (the `root_context`) we are able to generate
// a binry tree of a given depth. The root context can generate an arbitrarly large
// tree so we terminate the process when hitting a given `depth`.
tree_t *tree_from_context(child_context get_context, void *root_context, int depth);

#endif