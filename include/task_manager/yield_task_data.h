#ifndef YIELD_TASK_DATA_H
#define YIELD_TASK_DATA_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task_manager/task.h"
#include "../../include/task_manager/result.h"
#include "../../include/task_manager/task_manager.h"

// Function to add tasks_queue to the task manager
int yield_task_data(task_manager_t* manager, char* data, size_t length); 

#endif // YIELD_TASK_DATA_H