#ifndef CONTEXTS_H
#define CONTEXTS_H
#include "../utils/bin/bin.h"

// A context is a finite collection of data associated with every node in a binary
// tree. The idea is that, given the context of a node, we are able to decide if
// it has a left or right child, and, if so, produce the contexts for those children.
// Thus, given the context of the root node, we are able to generate an arbitrarly 
// large tree. See gen_tree.h for generating a tree from a root context.

// The tree generated by a context corresponds to a family of words satisfying 
// a combinatorial condition. 

////////////////////////////////////////////////////////////////////////////////

// Context object associated with binary words that satisfy the three gap
// condition. In particular, the gaps between consecutive ones belong to a set
// of size 3 and the gaps between between consecutive zeros belong to a set of size 3
typedef struct three_gap_context {
    bin_t *one_gaps;
    bin_t *zero_gaps;
    int index;
    int last_zero_index;
    int last_one_index;
} three_gap_context_t;

void *root_three_gap_context();

void *get_three_gap_context(void *context, int direction);

////////////////////////////////////////////////////////////////////////////////

typedef struct higher_gap_context {


} higher_gap_context_t;

////////////////////////////////////////////////////////////////////////////////

typedef struct recurring_gap_context {


} recurring_gap_context_t;


#endif