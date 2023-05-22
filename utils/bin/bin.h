#ifndef BIN_H
#define BIN_H
#include <stdbool.h>

// A set of elements with a fixed upper size
typedef struct bin {
    int size;
    int *elements;
} bin_t;

bin_t *new_bin(int size);

// Adds an element to the bin. The bin has a fixed capacity. If the 
// inserted element is already in the bin or if the bin is not at full
// capacity then returns true (insertion was successfull). Otherwise if
// the element was not in the bin before but the bin is at full capacity
// then return false
bool add(bin_t *set, int element);

#endif