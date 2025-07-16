#ifndef ENCODE_DATA_H
#define ENCODE_DATA_H

#include "stdio.h"
#include "stdlib.h"

#include "../../include/task/task.h"
#include "../../include/result/result.h"
#include "../../include/result/init_result.h"
#include "../../include/result/reallocate_result.h"
#include "../../include/result/free_result.h"

// Function to count run lengths for a input string
result_t* encode_data(task_t* task);

#endif // ENCODE_DATA_H