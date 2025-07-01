#ifndef TASK_DATA_H
#define TASK_DATA_H

#include <stdio.h>

typedef struct {
    char* data; 
    size_t size;
    size_t reserved_index;
} task_data_t;

#endif // TASK_DATA_H