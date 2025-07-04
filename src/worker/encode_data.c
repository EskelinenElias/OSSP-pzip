#include "../../include/worker/encode_data.h"

// Function to count run lengths for a input string
result_data_t* encode_data(task_data_t* task_data) {
    
    // Input validation 
    if (!task_data || (task_data->data == NULL && task_data->size > 0) || task_data->size < 0 || (task_data->data != NULL && task_data->size == 0)) {
        
        // Invalid input
        fprintf(stderr, "Failed to encode data. Invalid input\n");
        return NULL; 
    }
    
    // Allocate memory for the result data structure
    result_data_t* result_data = init_result_data(task_data->size);
    
    // If the input is empty (indicating EOF), return empty result data
    if (task_data->size == 0) return result_data;
    
    // Initialize the tracked character to the first character of the input and set count to 1
    char tracked_char = task_data->data[0];
    size_t tracked_count = 1;
    size_t index = 0; 
    
    // Count run lenghts of subsequent matching characters in the input
    for (size_t i = 1; i < task_data->size; i++) {
        
        // Check if the tracked character is the same as the current character in the input
        if (tracked_char == task_data->data[i]) {
            
            // Increment count if the characters match
            tracked_count++;
        
        } else {
            
            // Increase result capacity if necessary
            if (index == result_data->capacity) {
                
                // Double the capacity of the result
                if (reallocate_result_data(result_data, result_data->capacity * 2) != SUCCESS) {
                    
                    // Handle failure to increase capacity
                    fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
                    return NULL;
                }
            }
            
            // Append the tracked character and count to the result
            result_data->characters[index] = tracked_char;
            result_data->counts[index] = tracked_count;
            
            // Increment index
            index++;
            
            // Set the tracked character to the current character in the input and set count to 1
            tracked_char = task_data->data[i];
            tracked_count = 1;
        }
    }
    
    // Increase result capacity if necessary
    if (index == result_data->capacity) {
        
        // Double the capacity of the result
        if (reallocate_result_data(result_data, result_data->capacity * 2) != SUCCESS) {
            
            // Handle failure to increase capacity
            fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
            return NULL;
        }
    }

    // Append the tracked character and count to the output
    result_data->characters[index] = tracked_char;
    result_data->counts[index] = tracked_count;
    
    // Increment index
    index++;
    
    // Deallocate unused memory
    reallocate_result_data(result_data, index); 
    
    // Successfully encoded data
    return result_data; 
}

// EOF