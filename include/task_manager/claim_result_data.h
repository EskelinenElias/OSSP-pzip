#ifndef CLAIM_RESULT_DATA_H
#define CLAIM_RESULT_DATA_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task_manager/task.h"
#include "../../include/task_manager/result.h"
#include "../../include/task_manager/task_manager.h"

// Function to claim a result from the manager
result_data_t* claim_result_data(task_manager_t* manager);

#endif // CLAIM_RESULT_DATA_H