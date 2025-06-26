#ifndef YIELD_RESULT_H
#define YIELD_RESULT_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task_manager/task.h"
#include "../../include/task_manager/result.h"
#include "../../include/task_manager/task_manager.h"

// Function to yield results_queue to the manager
int yield_result(task_manager_t* manager, result_t* result, result_data_t* result_data);

#endif // YIELD_RESULT_H