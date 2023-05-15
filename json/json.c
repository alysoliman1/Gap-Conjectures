#include <stdio.h>
#include "json.h"

void dump_array(char *filename, int *array, int length){
    FILE *stream = fopen(filename, "w");
    fprintf(stream, "[");
    for (int i = 0; i < length - 1; i ++){
        fprintf(stream, "%d,", array[i]);
    }
    fprintf(stream, "%d]", array[length - 1]);
    fclose(stream);
}