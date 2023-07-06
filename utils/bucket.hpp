#ifndef BUCKET_HPP
#define BUCKET_HPP
#include <vector>

class bucket
{
    private:
        int size;
        std::vector<int> elements;

    public:
        bucket(int size){
            this->size = size;
        }
        bucket *copy();
        bool insert(int new_element);
};

#endif