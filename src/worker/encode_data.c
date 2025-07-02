#include "../../include/worker/encode_data.h"

// Function to count run lengths for a input string
result_t* encode_data(task_t* task) {
    
    // Input validation 
    if (!task || (task->data == NULL && task->size > 0) || task->size < 0 || (task->data != NULL && task->size == 0)) {
        
        // Invalid input
        fprintf(stderr, "Failed to encode data. Invalid input\n");
        return NULL; 
    }
    
    // Allocate memory for the result data structure
    result_t* result = init_result(task->size);
    
    // If the input is empty (indicating EOF), return empty result data
    if (task->size == 0) return result;
    
    // Initialize the tracked character to the first character of the input and set count to 1
    char tracked_char = task->data[0];
    size_t tracked_count = 1;
    size_t index = 0; 
    
    // Count run lenghts of subsequent matching characters in the input
    for (size_t i = 1; i < task->size; i++) {
        
        // Check if the tracked character is the same as the current character in the input
        if (tracked_char == task->data[i]) {
            
            // Increment count if the characters match
            tracked_count++;
        
        } else {
            
            // Increase result capacity if necessary
            if (index == result->capacity) {
                
                // Double the capacity of the result
                if (reallocate_result(result, result->capacity * 2) != SUCCESS) {
                    
                    // Handle failure to increase capacity
                    fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
                    return NULL;
                }
            }
            
            // Append the tracked character and count to the result
            result->characters[index] = tracked_char;
            result->counts[index] = tracked_count;
            
            // Increment index
            index++;
            
            // Set the tracked character to the current character in the input and set count to 1
            tracked_char = task->data[i];
            tracked_count = 1;
        }
    }
    
    // Increase result capacity if necessary
    if (index == result->capacity) {
        
        // Double the capacity of the result
        if (reallocate_result(result, result->capacity * 2) != SUCCESS) {
            
            // Handle failure to increase capacity
            fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
            return NULL;
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
    return result; 
}

// EOF