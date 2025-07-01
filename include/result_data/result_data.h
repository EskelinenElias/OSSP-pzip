#ifndef RESULT_DATA_H
#define RESULT_DATA_H

#include <stdlib.h>

typedef struct {
    char* characters; 
    size_t* counts; 
    size_t capacity;
} result_data_t; 

#endif // RESULT_DATA_H