#ifndef YIELD_TASK_H
#define YIELD_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/task/task.h"

// Function to yield task to tasks queue
int yield_task(tasks_queue_t* tasks_queue, task_t* task); 

#endif // YIELD_TASK_H