#ifndef ENCODING_TASK_H
#define ENCODING_TASK_H

#include <stdio.h>

// Structure to represent encoding task
typedef struct {
    char* data; 
    size_t size;
    size_t reserved_index;
} encoding_task_t;

#endif // ENCODING_TASK_H