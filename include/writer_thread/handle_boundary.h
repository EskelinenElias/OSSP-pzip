#ifndef HANDLE_BOUNDARY_H
#define HANDLE_BOUNDARY_H

#include "stdio.h"
#include "stdlib.h"

#include "../constants.h"
#include "../encoding_result/encoding_result.h"

// Function to handle the boundary condition between two consecutive encoding results
int handle_boundary(encoding_result_t* left, encoding_result_t* right); 

#endif // HANDLE_BOUNDARY_H

