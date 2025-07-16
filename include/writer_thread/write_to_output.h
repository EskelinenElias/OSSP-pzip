#ifndef WRITE_TO_OUTPUT_H
#define WRITE_TO_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"

#include "../encoding_result/encoding_result.h"

// Function to write encoded data to standard output as bytes
int write_encoding_result_to_output(encoding_result_t* data);

#endif // WRITE_TO_OUTPUT_H