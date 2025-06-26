#ifndef ENCODE_H
#define ENCODE_H

#include "stdio.h"
#include "stdlib.h"

#include "../../include/task_manager/task.h"
#include "../../include/task_manager/result.h"

// Function to count run lengths for a input string
result_data_t* encode(task_data_t* task_data);

#endif // ENCODE_H