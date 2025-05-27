#include "../include/rle_input.h"

// Function to create a new RLE input object
rle_input_t create_rle_input(char* input, size_t lenght) {
    return (rle_input_t){input, lenght}; 
}