#include "contexts.hpp"

context *recurring_gap::helper(int x){
   // Make a copy of the current context
    recurring_gap *ctx = new recurring_gap(recurring_gap::gap, recurring_gap::cap);
    for (int i = 0; i < recurring_gap::cap; i++)
    {
        ctx->L[i] = recurring_gap::L[i];
        ctx->T[i] = recurring_gap::T[i];
        ctx->R[i] = recurring_gap::R[i];
        ctx->S[i] = recurring_gap::S[i]->copy();
    }

    // Apply procedure
    int y;
    for (int i = 0; i < recurring_gap::cap; i++)
    {
        if (ctx->T[i] == x && ctx->R[i] >= 0)
        {
            y = ctx->L[i] - ctx->R[i];
            if (!ctx->S[i]->insert(y)){
                delete ctx;
                return nullptr;
            }
            x = y;
            ctx->R[i] = ctx->L[i];
            ctx->L[i] += 1;
        } else 
        {
            if (ctx->T[i] == x)
            {
                ctx->R[i] = ctx->L[i];
            } else 
            {
                ctx->T[i] = x;
            }
            ctx->L[i] += 1;
            break;
        }
    }
    return ctx;
}

context *recurring_gap::Left(){
    return recurring_gap::helper(0);
}

context *recurring_gap::Right(){
    return recurring_gap::helper(1);
}