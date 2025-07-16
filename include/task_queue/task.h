#ifndef TASK_H
#define TASK_H

#include <stdio.h>

// Structure to represent a task
typedef struct {
    char* data; 
    size_t size;
    size_t reserved_index;
} task_t;

#endif // TASK_H