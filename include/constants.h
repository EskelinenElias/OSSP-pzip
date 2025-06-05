#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <unistd.h>

// Define boolean values
#define TRUE  1
#define FALSE 0

// Define return types
#define SUCCESS 0
#define FAILURE 1
#define ERROR 1

// Parallel processing constants
#define MAX_CHUNK_SIZE 1024 * 10

// Get the number of available CPU cores
static inline int get_num_cores() {
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    return (num_cores > 0) ? (int)num_cores : 1;
}

#endif // CONSTANTS_H