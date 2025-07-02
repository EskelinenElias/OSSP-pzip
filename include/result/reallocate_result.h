#ifndef REALLOCATE_RESULT_H
#define REALLOCATE_RESULT_H

#include <stdio.h>
#include <stdlib.h>

#include "../../include/constants.h"
#include "../../include/result/result.h"

// Function to reallocate result data size
int reallocate_result(result_t* result, size_t new_capacity); 

#endif // REALLOCATE_RESULT_H