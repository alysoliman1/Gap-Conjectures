#ifndef BIN_H
#define BIN_H
#include <stdbool.h>

// A set of elements with a fixed upper size
typedef struct bin {
    int size;
    int *elements;
} bin_t;

// Create a heap object from heap memory
bin_t *new_bin(int size);

// Free up a bin from heap memory
void free_bin(bin_t *bin);

// Create a deepy copy of a bin object
bin_t *copy_bin(bin_t *src);

// Adds an element to the bin. The bin has a fixed capacity. If the 
// inserted element is already in the bin or if the bin is not at full
// capacity then returns true (insertion was successfull). Otherwise if
// the element was not in the bin before and the bin is at full capacity
// then return false (insertion was not successfull)
bool add(bin_t *bin, int element);

#endif