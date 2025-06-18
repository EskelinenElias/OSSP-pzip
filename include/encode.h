#ifndef ENCODE_H
#define ENCODE_H

#include "stdio.h"
#include "stdlib.h"
#include "../include/result.h"

// Function to count run lengths for a input string
int encode(char* input, size_t length, result_t* result);

// Function to handle the boundary on subsequent run lenght objects
int handle_boundary(result_t* left_obj, result_t* right_obj); 

#endif // ENCODE_H