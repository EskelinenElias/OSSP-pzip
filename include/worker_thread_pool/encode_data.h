#ifndef ENCODE_DATA_H
#define ENCODE_DATA_H

#include "stdio.h"
#include "stdlib.h"

#include "../task_queue/task.h"
#include "../result_queue/result.h"
#include "../result_queue/init_result.h"
#include "../result_queue/reallocate_result.h"
#include "../result_queue/free_result.h"

// Function to count run lengths for a input string
result_t* encode_data(task_t* task);

#endif // ENCODE_DATA_H