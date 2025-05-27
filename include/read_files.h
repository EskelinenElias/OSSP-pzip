#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "../include/constants.h"

// Function that reads data from a file into a buffer until EOF or the buffer is full
size_t read_to_buffer(FILE *input, char *buffer_start, char *buffer_end);

#endif // FILES_H