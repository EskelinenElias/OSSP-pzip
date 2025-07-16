#ifndef CLAIM_TASK_H
#define CLAIM_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "../encoding_task/encoding_task.h"
#include "../encoding_task/free_encoding_task.h"

#include "task_queue.h"

// Function to claim task from task queue
encoding_task_t* claim_task(task_queue_t* task_queue); 

#endif // CLAIM_TASK_H