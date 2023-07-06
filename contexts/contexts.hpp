#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <vector>
#include "../utils/bucket.hpp"

// An object implementing the context interface is a finite collection of data 
// associated with every node in a binary tree. Given the context at a node, we 
// are able to decide if the node has a left or right child, and, if so, produce
// contexts for those children. Thus, given the context at a root node, we are 
// able to generate an arbitrarly large binary tree.
class context {
    public:
        // If a node with the current context has a left child then the context 
        // for that child is returned. Otherwise if the node doesn't have a left
        // child then a null pointer is returned.
        virtual context *Left(){
            return nullptr;
        };

        // If a node with the current context has a left child then the context 
        // for that child is returned. Otherwise if the node doesn't have a left
        // child then a null pointer is returned.
        virtual context *Right(){
            return nullptr;
        };
};

// Below are some implementations of the context interface

class recurring_gap : public context {
    private:
        int cap;
        int gap;
        int *L;
        int *T;
        int *R;
        bucket **S;

    private:
        context *helper(int x);

    public:
        recurring_gap(int gap, int cap){
            this->cap = cap;
            this->gap = gap;
            this->L = new int[cap];
            this->T = new int[cap];
            this->R = new int[cap];
            this->S = new bucket*[cap];
            for (int i = 0; i < cap; i++){
                this->L[i] = 0;
                this->T[i] = 0;
                this->R[i] = -1;
                this->S[i] = new bucket(gap);
            }
        }

        ~recurring_gap(){
            delete [] this->L;
            delete [] this->T;
            delete [] this->R;
            for (int i = 0; i < cap; i++){
                delete this->S[i];
            }
            delete []this->S;
        }

        virtual context *Left();
        virtual context *Right();
};

class higher_gap : public context {

    private:
        int cap;
        int gap;
        


};

#endif