#include <cassert>
#include "bucket.hpp"

int main(){
    bucket *b;
    
    b = new bucket(0);
    assert(b->insert(0) == false);
    delete b;

    b = new bucket(1);
    assert(b->insert(0) == true);
    assert(b->insert(0) == true);
    assert(b->insert(1) == false);
    delete b;

    b = new bucket(4);
    assert(b->insert(0) == true);
    assert(b->insert(0) == true);
    assert(b->insert(1) == true);
    assert(b->insert(2) == true);
    assert(b->insert(3) == true);
    assert(b->insert(4) == false);
    assert(b->insert(2) == true);
    delete b;

    return 0;
}