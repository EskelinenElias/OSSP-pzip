#ifndef ENCODED_DATA_H
#define ENCODED_DATA_H

#include <stdlib.h>

typedef struct {
    char* characters; 
    size_t* counts; 
    size_t capacity;
} encoded_data_t; 

#endif // ENCODED_DATA_H