#ifndef CLAIM_TASK_H
#define CLAIM_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "task_queue.h"
#include "task.h"
#include "free_task.h"

// Function to claim task from task queue
task_t* claim_task(task_queue_t* task_queue); 

#endif // CLAIM_TASK_H