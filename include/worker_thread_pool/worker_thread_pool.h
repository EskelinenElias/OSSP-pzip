#ifndef WORKER_THREAD_POOL_H
#define WORKER_THREAD_POOL_H

#include <stdio.h>

#include "../tasks_queue/tasks_queue.h"

#include "worker_thread.h"

// Structure to represent worker thread pool
typedef struct {
    worker_thread_t** workers; 
    size_t num_workers; 
    tasks_queue_t* tasks_queue;
} worker_thread_pool_t;

#endif // WORKER_THREAD_POOL_H