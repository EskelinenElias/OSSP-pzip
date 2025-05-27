#include "../include/encoding_task.h"

// Function to create a new RLE compression task
encoding_task_t create_encoding_task(char* input, size_t length, encoded_data_t* output) {
    return (encoding_task_t){input, length, output};
}

// Function to generate a termination task 
encoding_task_t get_termination_task() {
    return (encoding_task_t){NULL, 0}; 
}