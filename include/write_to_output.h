#ifndef WRITE_TO_OUTPUT_H
#define WRITE_TO_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/constants.h"
#include "../include/encoded_data.h"

// Function to write encoded data to standard output as bytes
int write_encoded_data_to_output(encoded_data_t* data);

// Function to write encoded data to standard output as text (for testing purposes)
int write_encoded_text_to_output(encoded_data_t* data); 

#endif // WRITE_TO_OUTPUT_H