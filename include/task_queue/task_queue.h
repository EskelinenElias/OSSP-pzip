#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <stdio.h>
#include <pthread.h>

#include "../constants.h"

#include "../result_queue/result_queue.h"
#include "../encoding_task/encoding_task.h"

// Structure to represent task queue
typedef struct {
    pthread_mutex_t* lock;                  // Mutex for thread-safe access to the queue
    size_t head, tail, size, capacity;      // Queue management
    pthread_cond_t* room_available;         // Condition variable for signaling when there is room available in the queue
    pthread_cond_t* tasks_available;        // Condition variable for signaling when tasks are available
    encoding_task_t** tasks;                // Array of encoding tasks
    result_queue_t* result_queue;           // Pointer to result queue
} task_queue_t;

#endif // TASK_QUEUE_H