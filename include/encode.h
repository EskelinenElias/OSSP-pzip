#ifndef ENCODE_H
#define ENCODE_H

#include "stdio.h"
#include "stdlib.h"

#include "../include/task.h"
#include "../include/result.h"

// Function to count run lengths for a input string
result_data_t* encode(task_data_t* task_data);

// Function to handle the boundary on subsequent run lenght objects
int handle_boundary(result_data_t* left, result_data_t* right); 

#endif // ENCODE_H