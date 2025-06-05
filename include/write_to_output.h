#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/constants.h"
#include "../include/encoded_data.h"

// Function to write compressed data to output as bytes
int write_encoded_data_to_output(FILE* output, encoded_data_t* data);

// Function to write compressed data to buffer as text (for testing purposes)
int write_encoded_text_to_output(FILE* output, encoded_data_t* data); 

#endif // OUTPUT_H