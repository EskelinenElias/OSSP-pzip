#ifndef INIT_TASK_QUEUE_H
#define INIT_TASK_QUEUE_H

#include <stdio.h>

#include "../result_queue/result_queue.h"

#include "task_queue.h"
#include "free_task_queue.h"

// Function to initialize task queue
task_queue_t* init_task_queue(size_t capacity, result_queue_t* result_queue);

#endif // INIT_TASK_QUEUE_H