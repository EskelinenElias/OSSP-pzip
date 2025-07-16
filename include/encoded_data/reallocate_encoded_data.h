#ifndef REALLOCATE_ENCODED_DATA_H
#define REALLOCATE_ENCODED_DATA_H

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"

#include "encoded_data.h"

// Function to reallocate encoded data size
int reallocate_encoded_data(encoded_data_t* encoded_data, size_t new_capacity); 

#endif // REALLOCATE_ENCODED_DATA_H