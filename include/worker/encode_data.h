#ifndef ENCODE_H
#define ENCODE_H

#include "stdio.h"
#include "stdlib.h"

#include "../../include/task_data/task_data.h"
#include "../../include/result_data/result_data.h"
#include "../../include/result_data/init_result_data.h"
#include "../../include/result_data/reallocate_result_data.h"
#include "../../include/result_data/free_result_data.h"

// Function to count run lengths for a input string
result_data_t* encode_data(task_data_t* task_data);

#endif // ENCODE_H