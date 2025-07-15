#ifndef WRITER_RESOURCES_H
#define WRITER_RESOURCES_H

#include <stdio.h>

#include "../result/result.h"

// Structure to represent writer thread resources
typedef struct {
    result_t* current_result; 
    result_t* next_result; 
} writer_thread_resources_t;

#endif // WRITER_RESOURCES_H