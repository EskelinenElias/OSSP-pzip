#ifndef CLAIM_TASK_H
#define CLAIM_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/task_data/task_data.h"
#include "../../include/task_data/free_task_data.h"

// Function to claim task from tasks queue
task_data_t* claim_task(tasks_queue_t* tasks_queue); 

#endif // CLAIM_TASK_H