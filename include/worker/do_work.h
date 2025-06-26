#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>

#include "../../include/constants.h"
#include "../../include/worker/encode.h"
#include "../../include/task_manager/task_manager.h"
#include "../../include/task_manager/claim_task.h"
#include "../../include/task_manager/yield_result.h"

// Structure to hold worker arguments
typedef struct {
    task_manager_t* task_manager;
} worker_args_t; 

// Function to process the input in threads
void* do_work(void* args); 


// int terminate_workers(pthread_t *threads, size_t num_workers, task_manager_t *queue);

#endif // WORKER_H