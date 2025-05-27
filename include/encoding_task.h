#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encoded_data.h"

// Task structure
typedef struct {
    char *input;           // Pointer to an input chunk
    size_t length;   // Length of input chunk
    encoded_data_t *output;    // Pointer to a RLE data array
} encoding_task_t;

// Function to initialize a task
encoding_task_t create_encoding_task(char* input, size_t length, encoded_data_t* output);

// Function to get termination task
encoding_task_t get_termination_task(); 

#endif // TASK_H