#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/constants.h"
#include "../include/encoded_data.h"

// Function to write compressed data to output as bytes
char* write_data_to_buffer(char* buffer, encoded_data_t data); 

// Function to write compressed data to buffer as text (for testing purposes)
void write_text_to_buffer(FILE* buffer, encoded_data_t data); 

#endif // OUTPUT_H