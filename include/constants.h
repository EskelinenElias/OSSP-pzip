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

// Encoding task status codes
#define COMPLETED 1
#define FAILED 2
#define IN_PROGRESS 0

// Parallel processing constants
#define MAX_CHUNK_SIZE 1024 * 10
#define TASK_SIZE 1024 * 10

#define MAX_THREADS 128

#define MAX_CAPACITY 1000

// Get the number of available CPU cores
static inline size_t get_num_cores() {
    size_t num_cores = fmin(sysconf(_SC_NPROCESSORS_ONLN), MAX_THREADS);
    return (num_cores > 0) ? num_cores : 1;
}

#endif // CONSTANTS_H