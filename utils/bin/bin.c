#include <stdlib.h>
#include <stdbool.h>
#include "bin.h"

bin_t *new_bin(int size){
    bin_t *bin = malloc(sizeof(bin_t));
    bin->size = size;
    bin->elements = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++){
        bin->elements[i] = -1;
    }
    return bin;
}

bool add(bin_t *bin, int element){
    // The assumption is that prior to calling this function,
    // bin->elements is of the form a1, a2, ..., an, -1, -1, ..., -1
    // (i.e all the non-negative integers are at the begining, followed
    // by a tail of negatives ones). This structure is maintained after 
    // element is inserted
    for (int i = 0; i < bin->size; i++){
        if (element == bin->elements[i]){
            return true;
        }
        if (bin->elements[i] == -1){
            bin->elements[i] = element;
            return true;
        }
    }
    return false;
}