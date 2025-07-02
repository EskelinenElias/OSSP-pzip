#ifndef RESULT_H
#define RESULT_H

#include <stdlib.h>

typedef struct {
    char* characters; 
    size_t* counts; 
    size_t capacity;
} result_t; 

#endif // RESULT_H