#include "../include/encode.h"

// Function to count run lengths for a input string
int encode(char* input, size_t length, result_t* result) {
    
    // Input validation 
    if (input == NULL || length <= 0 || result == NULL) {
        
        // Invalid input
        fprintf(stderr, "Failed to encode data. Invalid input\n");
        return FAILURE; 
    }
    
    // Initialize the tracked character to the first character of the input and set count to 1
    char tracked_char = input[0];
    size_t tracked_count = 1;
    size_t index = 0; 
    
    // Count run lenghts of subsequent matching characters in the input
    for (size_t i = 1; i < length; i++) {
        
        // Check if the tracked character is the same as the current character in the input
        if (tracked_char == input[i]) {
            
            // Increment count if the characters match
            tracked_count++;
        
        } else {
            
            // Increase result capacity if necessary
            if (index == result->capacity) {
                
                // Double the capacity of the result
                if (reallocate_result(result, result->capacity * 2) != SUCCESS) {
                    
                    // Handle failure to increase capacity
                    fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
                    return ERROR;
                }
            }
            
            // Append the tracked character and count to the result
            result->characters[index] = tracked_char;
            result->counts[index] = tracked_count;
            
            // Increment index
            index++;
            
            // Set the tracked character to the current character in the input and set count to 1
            tracked_char = input[i];
            tracked_count = 1;
        }
    }
    
    // Increase result capacity if necessary
    if (index == result->capacity) {
        
        // Double the capacity of the result
        if (reallocate_result(result, result->capacity * 2) != SUCCESS) {
            
            // Handle failure to increase capacity
            fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
            return ERROR;
        }
    }

    // Append the tracked character and count to the output
    result->characters[index] = tracked_char;
    result->counts[index] = tracked_count;
    
    // Increment index
    index++;
    
    // Deallocate unused memory
    reallocate_result(result, index); 
    
    // Successfully encoded data
    return SUCCESS; 
}

// Function to handle the boundary on subsequent results 
int handle_boundary(result_t* left, result_t* right) {
    
    // Check if the left and right object contain data
    if (left == NULL || right == NULL || left->capacity == 0 || right->capacity == 0) {
        return SUCCESS; 
    }

    // Get the last character from the left object
    char left_char = left->characters[left->capacity - 1]; 
    
    // Get the first character from the right object
    char right_char = right->characters[0]; 
    
    // Check if the left and right characters match
    if (left_char == right_char) {
                
        // Get the last count from the left object
        size_t* left_count = &left->counts[left->capacity - 1]; 
        
        // Get a pointer to the first count at the right object
        size_t* right_count = &right->counts[0]; 
        
        // Increase the count at the right object
        *right_count += *left_count; 
        *left_count = 0; 
        
        // Decrease the size of the left object
        left->capacity--; 
    }
    
    return SUCCESS; 
}