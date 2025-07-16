#ifndef YIELD_TASK_H
#define YIELD_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "task_queue.h"
#include "init_task.h"

// Function to yield task to task queue
int yield_task(task_queue_t* task_queue, char* task, size_t size); 

#endif // YIELD_TASK_H