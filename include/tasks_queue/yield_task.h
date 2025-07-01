#ifndef YIELD_TASK_H
#define YIELD_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/task_data/task_data.h"

// Function to yield task to tasks queue
int yield_task(tasks_queue_t* tasks_queue, task_data_t* task_data); 

#endif // YIELD_TASK_H