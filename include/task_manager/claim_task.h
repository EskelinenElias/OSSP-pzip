#ifndef CLAIM_TASK_H
#define CLAIM_TASK_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task_manager/task.h"
#include "../../include/task_manager/result.h"
#include "../../include/task_manager/task_manager.h"

// Function to claim a task from the manager
task_t* claim_task(task_manager_t* manager); 

#endif // CLAIM_TASK_H