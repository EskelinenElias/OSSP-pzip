#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>

#include "../../include/constants.h"
#include "../../include/worker/encode_data.h"
#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/tasks_queue/claim_task.h"
#include "../../include/results_queue/results_queue.h"
#include "../../include/results_queue/yield_result.h"
#include "../../include/result/free_result.h"
#include "../../include/task/free_task.h"

// Structure to hold worker arguments
typedef struct {
    tasks_queue_t* tasks_queue;
    results_queue_t* results_queue; 
} worker_args_t; 

// Function to process the input in threads
void* process_tasks(void* args); 

#endif // WORKER_H