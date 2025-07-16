#ifndef ENCODING_RESULT_H
#define ENCODING_RESULT_H

#include <stdlib.h>

typedef struct {
    char* characters; 
    size_t* counts; 
    size_t capacity;
} encoding_result_t; 

#endif // ENCODING_RESULT_H