#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>

#include "../../include/constants.h"
#include "../../include/task_manager/result.h"

// Input data structure; represents a chunk of input data
typedef struct {
    char* data; 
    size_t size;
} task_data_t;

typedef struct {
    task_data_t task_data;
    result_t* result;
} task_t;

// Function to initialize a task
task_t* init_task(char* data, size_t size, result_t* result);

// Function to free the memory allocated for an encoding task structure
void* free_task(task_t* task);

#endif // TASK_H