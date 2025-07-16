#include "../../include/worker_thread_pool/encode_data.h"

// Function to count run lengths for a input string
encoding_result_t* encode_data(char* data, size_t size) {
    
    // Input validation 
    if ((!data && size > 0) || size < 0 || (data && size == 0)) {
        
        // Invalid input
        fprintf(stderr, "Failed to encode data: invalid input\n");
        return NULL; 
    }
    
    // Initialize encoded data
    encoding_result_t* encoding_result = init_encoding_result(size);
    
    // If the input is empty (indicating EOF), return empty encoded data
    if (size == 0) return encoding_result;
    
    // Initialize the tracked character to the first character of the input and set count to 1
    char tracked_char = data[0];
    size_t tracked_count = 1;
    size_t index = 0; 
    
    // Count run lenghts of subsequent matching characters in the input
    for (size_t i = 1; i < size; i++) {
        
        // Check if the tracked character is the same as the current character in the input
        if (tracked_char == data[i]) {
            
            // Increment count if the characters match
            tracked_count++;
        
        } else {
            
            // Increase encoding_result capacity if necessary
            if (index == encoding_result->capacity) {
                
                // Double the capacity of the encoding_result
                if (resize_encoding_result(encoding_result, encoding_result->capacity * 2) != SUCCESS) {
                    
                    // Handle failure to increase capacity
                    fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
                    return NULL;
                }
            }
            
            // Append the tracked character and count to the encoding_result
            encoding_result->characters[index] = tracked_char;
            encoding_result->counts[index] = tracked_count;
            
            // Increment index
            index++;
            
            // Set the tracked character to the current character in the input and set count to 1
            tracked_char = data[i];
            tracked_count = 1;
        }
    }
    
    // Increase encoding_result capacity if necessary
    if (index == encoding_result->capacity) {
        
        // Double the capacity of the encoding_result
        if (resize_encoding_result(encoding_result, encoding_result->capacity * 2) != SUCCESS) {
            
            // Handle failure to increase capacity
            fprintf(stderr, "Failed to encode data. Failed to increase capacity\n");
            return NULL;
        }
    }

    // Append the tracked character and count to the output
    encoding_result->characters[index] = tracked_char;
    encoding_result->counts[index] = tracked_count;
    
    // Increment index
    index++;
    
    // Deallocate unused memory
    resize_encoding_result(encoding_result, index); 
    
    // Successfully encoded data
    return encoding_result; 
}

// EOF