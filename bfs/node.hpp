#ifndef NODE_HPP
#define NODE_HPP
#include <vector>
#include "../contexts/contexts.hpp"

class node
{   
    public:
        int value;
        int layer;
        std::vector<context *> *ctxs;
    public:
        node(std::vector<context *> *ctxs, int value, int layer)
        {
            this->ctxs = ctxs;   
            this->layer = layer;
        }

        ~node(){
            for (context *ctx : *(this->ctxs))
            {
                delete ctx;
            }
            delete this->ctxs;
        }

        node *get_left();
        node *get_right();
};


#endif