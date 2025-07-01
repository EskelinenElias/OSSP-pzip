#ifndef RESULTS_QUEUE_H
#define RESULTS_QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/result_data/result_data.h"

// Structure of the results queue
typedef struct {
    pthread_mutex_t* lock;                  // Mutex for thread-safe access to the queue
    size_t front, rear, size, capacity;     // Queue management
    pthread_cond_t* room_available;         // Condition variable for signaling when there is room available in the queue
    pthread_cond_t* result_available;       // Condition variable for signaling when a result is available
    result_data_t** results;                // Array of pointers to result data
    int* status_flags;                      // Array of status flags indicating the status of each result
} results_queue_t;

#endif // RESULTS_QUEUE_H