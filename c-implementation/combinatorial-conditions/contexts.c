#include <stdlib.h>
#include "contexts.h"
#include "../utils/tree/tree.h"

void *new_recurring_gap_context(int *gap_bounds, int gaps_depth){
    recurring_gap_context_t *ctx = malloc(sizeof(recurring_gap_context_t));
    ctx->gaps_depth = gaps_depth;
    ctx->index = malloc(gaps_depth * sizeof(int));
    ctx->last_letter = malloc(gaps_depth * sizeof(int));
    ctx->last_recurrence = malloc(gaps_depth * sizeof(int));
    ctx->gap_bins = malloc(gaps_depth * sizeof(bin_t *));

    for (int i = 0; i < gaps_depth; i++){
        ctx->index[i] = -1;
        ctx->last_letter[i] = -1;
        ctx->last_recurrence[i] = -1;
        ctx->gap_bins[i] = new_bin(gap_bounds[i]);
    }

    return (void *)ctx;
}

recurring_gap_context_t *copy_recurring_gap_context(recurring_gap_context_t *src){
    recurring_gap_context_t *dst = malloc(sizeof(recurring_gap_context_t));
    dst->gaps_depth = src->gaps_depth;
    dst->index = malloc(src->gaps_depth * sizeof(int));
    dst->last_letter = malloc(src->gaps_depth * sizeof(int));
    dst->last_recurrence = malloc(src->gaps_depth * sizeof(int));
    dst->gap_bins = malloc(src->gaps_depth * sizeof(bin_t *));
    for (int i = 0; i < src->gaps_depth; i++){
        dst->index[i] = src->index[i];
        dst->last_letter[i] = src->last_letter[i];
        dst->last_recurrence[i] = src->last_recurrence[i];
        dst->gap_bins[i] = copy_bin(src->gap_bins[i]);
    } 
    return dst;
}

void *get_recurring_gap_context(void *context, int direction){
    // Make a copy of the context
    recurring_gap_context_t *ctx = copy_recurring_gap_context((recurring_gap_context_t *)context);

    // Start procedure
    int new_letter = direction;
    int gap;
    for (int i = 0; i < ctx->gaps_depth; i++){
        if (new_letter == ctx->last_letter[i] && ctx->last_recurrence[i] != -1){
            gap = ctx->index[i] - ctx->last_recurrence[i];
            if (add(ctx->gap_bins[i], gap) == false){
                return NULL;
            }
            new_letter = gap;
            ctx->last_recurrence[i] = ctx->index[i];
            ctx->index[i] += 1;
        } else {
            if (new_letter == ctx->last_letter[i]){
                ctx->last_recurrence[i] = ctx->index[i];
            } else {
                ctx->last_letter[i] = new_letter;
            }
            ctx->index[i] += 1; 
            break;
        }
    }
    return (void *)ctx;
}