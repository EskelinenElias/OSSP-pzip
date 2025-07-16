#ifndef ENCODE_DATA_H
#define ENCODE_DATA_H

#include "stdio.h"
#include "stdlib.h"

#include "../encoding_result/init_encoding_result.h"
#include "../encoding_result/resize_encoding_result.h"
#include "../encoding_result/free_encoding_result.h"

// Function to encode data
encoding_result_t* encode_data(char* data, size_t size);

#endif // ENCODE_DATA_H