#ifndef INIT_WORKER_THREAD_POOL_H
#define INIT_WORKER_THREAD_POOL_H

#include <stdio.h>

#include "../tasks_queue/tasks_queue.h"
#include "../results_queue/results_queue.h"

#include "worker_thread_pool.h"
#include "free_worker_thread_pool.h"
#include "init_worker_thread.h"

// Function to initialize worker thread pool
worker_thread_pool_t* init_worker_thread_pool(size_t num_workers, tasks_queue_t* tasks_queue, results_queue_t* results_queue); 

#endif // INIT_WORKER_THREAD_POOL_H