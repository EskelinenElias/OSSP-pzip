#ifndef ENCODE_H
#define ENCODE_H

#include "stdio.h"
#include "stdlib.h"
#include "../include/encoded_data.h"

// Function to count run lengths for a input string
int encode(char* input, size_t length, encoded_data_t* output);

// Function to handle the boundary on subsequent run lenght objects
int handle_boundary(encoded_data_t* left_obj, encoded_data_t* right_obj); 

#endif // ENCODE_H