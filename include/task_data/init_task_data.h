#ifndef INIT_TASK_DATA_H
#define INIT_TASK_DATA_H

#include <stdlib.h>

#include "../../include/task_data/task_data.h"

// Function to initialize task data
task_data_t* init_task_data(char* data, size_t size, size_t reserved_index);

#endif // INIT_TASK_DATA_H