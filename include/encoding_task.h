#ifndef ENCODING_TASK_H
#define ENCODING_TASK_H

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encoded_data.h"

// Input data structure; represents a chunk of input data
typedef struct {
    char *data;                 // Pointer to the start of an input string 
    size_t length;              // Length of input string to process
} input_data_t;

// Encoding task structure; represents a chunk of data to be encoded
typedef struct {
    atomic_int status;              // Status of the task
    char* input;                // Pointer to input data
    size_t length;              // Length of input data to process
    encoded_data_t *output;     // Pointer to an output encoded data stucture 
} encoding_task_t;

// Function to initialize a task
encoding_task_t* create_encoding_task(char* input, size_t length);

// Function to create an empty encoding task 
encoding_task_t* create_empty_encoding_task();

// Function to free the memory allocated for an encoding task structure
void free_encoding_task(encoding_task_t* task);

// Function to get termination task
encoding_task_t get_termination_task(); 

#endif // ENCODING_TASK_H