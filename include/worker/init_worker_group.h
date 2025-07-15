#ifndef INIT_WORKER_GROUP_H
#define INIT_WORKER_GROUP_H

#include <stdio.h>

#include "../tasks_queue/tasks_queue.h"
#include "../results_queue/results_queue.h"

#include "worker_group.h"
#include "init_worker_thread.h"
#include "free_worker_group.h"

// Function to initialize worker thread
worker_group_t* init_worker_group(size_t num_workers, tasks_queue_t* tasks_queue, results_queue_t* results_queue); 

#endif // INIT_WORKER_THREAD_H