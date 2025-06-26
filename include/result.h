#ifndef RESULT_H
#define RESULT_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/constants.h"

typedef struct {
    char* characters; 
    size_t* counts; 
    size_t capacity;
} result_data_t; 

typedef struct {
    int status; 
    result_data_t* data;
} result_t; 

// Function to initialize a new result structure
result_t* init_result(); 

// Function to free a result structure
void* free_result(result_t* result); 

// Function to initialize a new result data structure
result_data_t* init_result_data(size_t initial_capacity); 

// Function to increase result data structure capacity
int reallocate_result_data(result_data_t* result, size_t new_capacity); 

// Function to free the memory allocated for a result data structure
void* free_result_data(result_data_t* result); 

#endif // RESULT_H