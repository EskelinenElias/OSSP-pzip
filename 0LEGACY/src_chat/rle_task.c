#include "../include/rle_task.h"

// Function to create a new RLE compression task
rle_task_t create_rle_task(char* input, size_t length, rle_data_t* output) {
    return (rle_task_t){input, length, output};
}

// Function to generate a termination task 
rle_task_t get_termination_task() {
    return (rle_task_t){NULL, 0}; 
}