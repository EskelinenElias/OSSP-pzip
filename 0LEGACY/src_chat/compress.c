#include "stdio.h"
#include "stdlib.h"
#include "../include/constants.h"
#include "../include/compress.h"

// Function to count run lengths for a input string
int count_run_lengths(char* input, size_t length, rle_data_t* output) {
    
    // Input validation 
    if (input == NULL || length <= 0 || output == NULL) {
        
        // Invalid input
        fprintf(stderr, "count_run_lengths: Invalid input.\n");
        return FAILURE; // Error
    }
    
    // Initialize the tracked character to the first character of the input and set count to 1
    char tracked_char = input[0];
    size_t tracked_count = 1;
    
    // Count run lenghts of subsequent matching characters in the input
    for (size_t i = 1; i < length; i++) {
        
        // Check if the tracked character is the same as the current character in the input
        if (tracked_char == input[i]) {
            
            // Increment count if the characters match
            tracked_count++;
        
        } else {
            
            // Append the tracked character and count to the output
            if (append_to_rle_data(output, tracked_char, tracked_count) != SUCCESS) {
                return FAILURE; // Error
            }
            
            // Set the tracked character to the current character in the input and set count to 1
            tracked_char = input[i];
            tracked_count = 1;
        }
    }

    // Append the tracked character and count to the output
    if (append_to_rle_data(output, tracked_char, tracked_count) != SUCCESS) {
        return FAILURE; 
    };
    
    return SUCCESS; // Success
}

// Function to handle the boundary on subsequent run lenght objects
int handle_boundary(rle_data_t* left_obj, rle_data_t* right_obj) {
    
    // Get the last character from the left object
    char left_char = left_obj->characters[left_obj->size - 1]; 
    
    // Get the first character from the right object
    char right_char = right_obj->characters[0]; 
    
    // Check if the left and right characters match
    if (left_char == right_char) {
                
        // Get the last count from the left object
        size_t left_count = left_obj->counts[left_obj->size - 1]; 
        
        // Get a pointer to the first count at the right object
        size_t* right_count = &right_obj->counts[0]; 
        
        //printf("L==R, L=%c, R=%c, %lu, %lu\n", left_char, right_char, left_count, *right_count);

        // Increase the count at the right object
        *right_count += left_count; 
        
        // Decrease the size of the left object
        left_obj->size--; 
    } else {
        //printf("L!=R, L=%c, R=%c\n", left_char, right_char); 
    }
    
    return SUCCESS; 
}