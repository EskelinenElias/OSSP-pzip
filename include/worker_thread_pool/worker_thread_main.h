#ifndef WORKER_THREAD_MAIN_H
#define WORKER_THREAD_MAIN_H

#include <stdio.h>

#include "../constants.h"

#include "../task_queue/task_queue.h"
#include "../task_queue/claim_task.h"
#include "../result_queue/result_queue.h"
#include "../result_queue/yield_result.h"
#include "../encoding_task/free_encoding_task.h"
#include "../encoding_result/free_encoding_result.h"

#include "worker_thread.h"
#include "encode_data.h"

// Function to clear worker thread resources (does not free the worker thread itself)
int clear_worker_thread_resources(worker_thread_t* worker); 

// Worker thread main function (processes task from task queue)
void* worker_thread_main(void* args); 

#endif // WORKER_THREAD_MAIN_H