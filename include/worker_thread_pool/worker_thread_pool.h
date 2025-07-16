#ifndef WORKER_THREAD_POOL_H
#define WORKER_THREAD_POOL_H

#include <stdio.h>

#include "../task_queue/task_queue.h"

#include "worker_thread.h"

// Structure to represent worker thread pool
typedef struct {
    worker_thread_t** workers; 
    size_t num_workers; 
    task_queue_t* task_queue;
} worker_thread_pool_t;

#endif // WORKER_THREAD_POOL_H