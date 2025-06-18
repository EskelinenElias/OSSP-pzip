#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/constants.h"
#include "../include/result.h"

// Input data structure; represents a chunk of input data
typedef struct {
    size_t index;               // Index of the task 
    char *data;                 // Pointer to the start of an input string 
    size_t length;              // Length of input string to process
} task_t;

// Function to initialize a task
task_t* create_task(char* input, size_t length, size_t index);

// Function to free the memory allocated for an encoding task structure
int free_task(task_t* task);

#endif // TASK_H