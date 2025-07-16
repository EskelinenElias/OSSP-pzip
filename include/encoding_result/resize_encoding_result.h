#ifndef RESIZE_ENCODING_RESULT_H
#define RESIZE_ENCODING_RESULT_H

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"

#include "encoding_result.h"

// Function to reallocate encoding result size
int resize_encoding_result(encoding_result_t* encoding_result, size_t new_capacity); 

#endif // RESIZE_ENCODING_RESULT_H