#ifndef WORKER_THREAD_MAIN_H
#define WORKER_THREAD_MAIN_H

#include <stdio.h>

#include "../constants.h"

#include "../tasks_queue/tasks_queue.h"
#include "../tasks_queue/claim_task.h"
#include "../results_queue/results_queue.h"
#include "../results_queue/yield_result.h"
#include "../result/free_result.h"
#include "../task/free_task.h"

#include "worker_thread.h"
#include "encode_data.h"

// Function to clear worker thread resources (does not free the worker thread itself)
int clear_worker_thread_resources(worker_thread_t* worker); 

// Worker thread main function (processes tasks from tasks queue)
void* worker_thread_main(void* args); 

#endif // WORKER_THREAD_MAIN_H