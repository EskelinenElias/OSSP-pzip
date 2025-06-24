#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/constants.h"
#include "../include/result.h"

// Input data structure; represents a chunk of input data
typedef struct {
    char* data; 
    size_t size;
    result_t** result; 
    int* status;
} task_t;

// Function to initialize a task
task_t* init_task(char* data, size_t size, result_t** result, int* status);

// Function to free the memory allocated for an encoding task structure
int free_task(task_t* task);

#endif // TASK_H