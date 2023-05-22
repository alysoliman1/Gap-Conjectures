#include <stdlib.h>
#include "contexts.h"
#include "../utils/tree/tree.h"

////////////////////////////////////////////////////////////////////////////////

void *root_three_gap_context(){
    three_gap_context_t *ctx = malloc(sizeof(three_gap_context_t));
    ctx->one_gaps = new_bin(3);
    ctx->zero_gaps = new_bin(3);
    ctx->index = 0;
    ctx->last_one_index = -1;
    ctx->last_zero_index = -1;
    return (void *)ctx;
}

void *get_three_gap_context(void *context, int direction){

}



////////////////////////////////////////////////////////////////////////////////