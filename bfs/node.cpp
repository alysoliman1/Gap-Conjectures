#include "node.hpp"

node *node::get_left(){
    std::vector<context *> *new_ctxs = new std::vector<context *>();
    context *new_ctx;
    for (context *ctx : *(node::ctxs)){
        new_ctx = ctx->Left();
        if (new_ctx == nullptr){
            delete new_ctxs;
            return nullptr;
        }
        new_ctxs->push_back(new_ctx);
    }
    return new node(new_ctxs, 0, node::layer + 1);
}

node *node::get_right(){
    std::vector<context *> *new_ctxs = new std::vector<context *>();
    context *new_ctx;
    for (context *ctx : *(node::ctxs)){
        new_ctx = ctx->Right();
        if (new_ctx == nullptr){
            delete new_ctxs;
            return nullptr;
        }
        new_ctxs->push_back(new_ctx);
    }
    return new node(new_ctxs, 1, node::layer + 1);
}