#ifndef RESULT_QUEUE_H
#define RESULT_QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "../encoding_result/encoding_result.h"

// Structure of the results queue
typedef struct {
    pthread_mutex_t* lock;                  // Mutex for thread-safe access to the queue
    size_t front, rear, size, capacity;     // Queue management
    pthread_cond_t* room_available;         // Condition variable for signaling when there is room available in the queue
    pthread_cond_t* result_available;       // Condition variable for signaling when result is available
    encoding_result_t** results;            // Array of pointers to encoding results
    int* status_flags;                      // Array of status flags indicating the status of each encoding result
} result_queue_t;

#endif // RESULT_QUEUE_H