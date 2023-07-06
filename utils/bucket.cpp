#include "bucket.hpp"

bucket *bucket::copy(){
    bucket *copy = new bucket(bucket::size);
    for (int element : bucket::elements){
        copy->elements.push_back(element);
    }
    return copy;
}

bool bucket::insert(int new_element){
    for (int element : bucket::elements){
        if (element == new_element){
            return true;
        }
    }

    if (bucket::elements.size() < bucket::size){
        bucket::elements.push_back(new_element);
        return true;
    }

    return false;
}