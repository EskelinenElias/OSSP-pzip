#ifndef CLAIM_TASK_H
#define CLAIM_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task/task.h"
#include "../../include/task/free_task.h"

#include "tasks_queue.h"

// Function to claim task from tasks queue
task_t* claim_task(tasks_queue_t* tasks_queue); 

#endif // CLAIM_TASK_H