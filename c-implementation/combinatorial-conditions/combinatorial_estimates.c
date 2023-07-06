#include "contexts.h"
#include "gen_tree.h"
#include "../utils/json/json.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
    clock_t start_time, stop_time;
    double time_elapsed;

/*
    void *TGT_ctx = new_three_gap_context();
    start_time = clock();
    tree_t *tree = tree_from_context(get_three_gap_context, TGT_ctx, 30);
    stop_time = clock();
    time_elapsed = (double) (stop_time - start_time) / CLOCKS_PER_SEC;
    printf("time elapsed: %f seconds\n", time_elapsed);    
    dump_array("upperbounds.json", tree->nodes_at_level, tree->depth);
*/
    int depth = 10;
    int *gap_bounds = malloc(depth * sizeof(int));
    gap_bounds[0] = 3;
    for (int i = 1; i < depth; i++){
        gap_bounds[i] = 1;
    }

    void *ctx = new_recurring_gap_context(gap_bounds, depth);
    start_time = clock();
    tree_t *tree = tree_from_context(get_recurring_gap_context, ctx, 100);
    stop_time = clock();
    time_elapsed = (double) (stop_time - start_time) / CLOCKS_PER_SEC;
    printf("time elapsed: %f seconds\n", time_elapsed);    
    dump_array("upperbounds.json", tree->nodes_at_level, tree->depth);

    return 0;
}