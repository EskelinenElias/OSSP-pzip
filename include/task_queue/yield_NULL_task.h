#ifndef YIELD_NULL_TASK_H
#define YIELD_NULL_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "task_queue.h"

// Function to yield NULL task to task queue
int yield_NULL_task(task_queue_t* task_queue); 

#endif // YIELD_NULL_TASK_H