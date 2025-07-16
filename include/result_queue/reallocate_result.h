#ifndef REALLOCATE_RESULT_H
#define REALLOCATE_RESULT_H

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"

#include "result.h"

// Function to reallocate result size
int reallocate_result(result_t* result, size_t new_capacity); 

#endif // REALLOCATE_RESULT_H