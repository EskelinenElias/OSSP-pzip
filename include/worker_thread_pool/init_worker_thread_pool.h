#ifndef INIT_WORKER_THREAD_POOL_H
#define INIT_WORKER_THREAD_POOL_H

#include <stdio.h>

#include "../task_queue/task_queue.h"
#include "../result_queue/result_queue.h"

#include "worker_thread_pool.h"
#include "free_worker_thread_pool.h"
#include "init_worker_thread.h"

// Function to initialize worker thread pool
worker_thread_pool_t* init_worker_thread_pool(size_t num_workers, task_queue_t* task_queue, result_queue_t* result_queue); 

#endif // INIT_WORKER_THREAD_POOL_H