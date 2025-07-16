#include "../../include/encoding_result/resize_encoding_result.h"

// Function to increase encoded data structure capacity
int resize_encoding_result(encoding_result_t* encoding_result, size_t new_capacity) {
    
    // Input validation
    if (encoding_result == NULL || new_capacity < 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to reallocate memory for encoded data: invalid input\n"); 
        return ERROR; 
    }
    
    // Initialize new arrays
    char* new_characters = NULL; 
    size_t* new_counts =  NULL; 
    
    // Check if the new capacity is greater than 0
    if (new_capacity > 0) {
        
        // Reallocate memory for characters
        if (!(new_characters = (char*)realloc(encoding_result->characters, new_capacity * sizeof(char)))) {
            
            // Failed to reallocate memory for characters
            fprintf(stderr, "Failed to reallocate memory for encoded data: failed to reallocate memory for characters\n"); 
            return ERROR; 
        }
        
        // Reallocate memory for counts
        if (!(new_counts = (size_t*)realloc(encoding_result->counts, new_capacity * sizeof(size_t)))) {
            
            // Failed to reallocate memory for counts
            fprintf(stderr, "Failed to reallocate memory for encoded data: failed to reallocate memory for counts\n"); 
            free(new_characters);
            return ERROR; 
        }
        
    } else {
        
        // Free the existing arrays
        free(encoding_result->characters); 
        free(encoding_result->counts); 
    }
    
    // Assign reallocated arrays to fields
    encoding_result->characters = new_characters; 
    encoding_result->counts = new_counts; 
    encoding_result->capacity = new_capacity; 
    
    // Successfully reallocated encoded data
    return SUCCESS;
}

// EOF