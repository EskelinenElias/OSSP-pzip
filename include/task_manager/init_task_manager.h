#ifndef INIT_TASK_MANAGER_H
#define INIT_TASK_MANAGER_H

#include "../../include/task_manager/task_manager.h"
#include "../../include/task_manager/free_task_manager.h"

// Function to initialize a task manager with a given capacity
task_manager_t* init_task_manager(size_t capacity);

#endif // INIT_TASK_MANAGER_H