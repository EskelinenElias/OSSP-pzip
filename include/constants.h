#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <unistd.h>
#include <math.h>

// Define boolean values
#define TRUE  1
#define FALSE 0

// Define return types
#define SUCCESS 0
#define FAILURE 2
#define ERROR 1
#define INVALID_INDEX -1

// Define encoding result status codes
#define COMPLETED 2
#define RESERVED 1
#define EMPTY 0

// Parallel processing constants
#define TASK_SIZE 1024
#define MAX_THREADS 16
#define QUEUE_CAPACITY 128

// Get the number of available CPU cores
static inline size_t get_num_cores() {
    
    // Determine the number of available CPU cores
    return sysconf(_SC_NPROCESSORS_ONLN);
}

#endif // CONSTANTS_H