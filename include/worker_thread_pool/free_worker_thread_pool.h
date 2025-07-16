#ifndef FREE_WORKER_THREAD_POOL_H
#define FREE_WORKER_THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../tasks_queue/tasks_queue.h"
#include "../tasks_queue/yield_task.h"

#include "worker_thread_pool.h"
#include "free_worker_thread.h"

// Function to free worker thread pool
int free_worker_thread_pool(worker_thread_pool_t* worker_pool); 

#endif // FREE_WORKER_THREAD_POOL_H