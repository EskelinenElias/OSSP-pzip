#ifndef RESULT_H
#define RESULT_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/constants.h"

// Task result
typedef struct {
    char* characters; 
    size_t* counts; 
    size_t capacity;
} result_t; 

// Function to initialize a new result data structure
result_t* init_result(size_t initial_capacity); 

// Function to increase result data structure capacity
int reallocate_result(result_t* result, size_t new_capacity); 

// Function to free the memory allocated for a result data structure
int free_result(result_t* result); 

#endif // RESULT_H