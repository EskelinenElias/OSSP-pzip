#ifndef TASKS_QUEUE_H
#define TASKS_QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task/task.h"

// Task manager
typedef struct {
    pthread_mutex_t* lock;                  // Mutex for thread-safe access to the queue
    size_t front, rear, size, capacity;     // Queue management
    pthread_cond_t* room_available;         // Condition variable for signaling when there is room available in the queue
    pthread_cond_t* tasks_available;        // Condition variable for signaling when tasks are available
    task_t** tasks;                         // Array of tasks
} tasks_queue_t;

#endif // TASKS_QUEUE_H