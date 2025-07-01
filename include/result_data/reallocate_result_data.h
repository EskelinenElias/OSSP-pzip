#ifndef REALLOCATE_RESULT_DATA_H
#define REALLOCATE_RESULT_DATA_H

#include <stdio.h>
#include <stdlib.h>

#include "../../include/constants.h"
#include "../../include/result_data/result_data.h"

// Function to reallocate result data size
int reallocate_result_data(result_data_t* result, size_t new_capacity); 

#endif // REALLOCATE_RESULT_DATA_H