#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "bin.h"

int main(){
    bin_t *bin = new_bin(0);
    assert(bin->size == 0);
    assert(add(bin, 0) == false);

    bin = new_bin(1);
    assert(bin->size == 1);
    assert(add(bin, 0) == true);
    assert(bin->elements[0] == 0);
    assert(add(bin, 0) == true);
    assert(add(bin, 1) == false);
    assert(bin->elements[0] == 0);

    bin = new_bin(4);
    assert(bin->size == 4);
    assert(add(bin, 0) == true);
    assert(add(bin, 0) == true);
    assert(bin->elements[0] == 0);
    assert(bin->elements[1] == -1);
    assert(bin->elements[2] == -1);
    assert(bin->elements[3] == -1);

    assert(add(bin, 1) == true);
    assert(bin->elements[0] == 0);
    assert(bin->elements[1] == 1);
    assert(bin->elements[2] == -1);
    assert(bin->elements[3] == -1);

    assert(add(bin, 2) == true);
    assert(bin->elements[0] == 0);
    assert(bin->elements[1] == 1);
    assert(bin->elements[2] == 2);
    assert(bin->elements[3] == -1);

    assert(add(bin, 3) == true);
    assert(bin->elements[0] == 0);
    assert(bin->elements[1] == 1);
    assert(bin->elements[2] == 2);
    assert(bin->elements[3] == 3);

    assert(add(bin, 4) == false);
    assert(add(bin, 2) == true);

    return 0;
}