#ifndef PROCESS_TASKS_H
#define PROCESS_TASKS_H

#include <stdio.h>

#include "../constants.h"

#include "../tasks_queue/tasks_queue.h"
#include "../tasks_queue/claim_task.h"
#include "../results_queue/results_queue.h"
#include "../results_queue/yield_result.h"
#include "../result/free_result.h"
#include "../task/free_task.h"

#include "encode_data.h"

// Structure to represent worker resources
typedef struct {
    task_t* task; 
    result_t* result; 
} worker_resources_t;

// Structure to represent worker arguments
typedef struct {
    tasks_queue_t* tasks_queue;
    results_queue_t* results_queue; 
} worker_args_t; 

// Function to initialize worker resources
worker_resources_t* init_worker_resources();

// Function to free worker resources
int free_worker_resources(worker_resources_t* resources); 

// Function to process the input in threads
void* process_tasks(void* args); 

#endif // PROCESS_TASKS_H