#include "../../include/result_data/reallocate_result_data.h"

// Function to increase result data structure capacity
int reallocate_result_data(result_data_t* result, size_t new_capacity) {
    
    // Input validation
    if (result == NULL || new_capacity < 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to reallocate memory for result data: invalid input\n"); 
        return ERROR; 
    }
    
    // Initialize new arrays
    char* new_characters = NULL; 
    size_t* new_counts =  NULL; 
    
    // Check if the new capacity is greater than 0
    if (new_capacity > 0) {
        
        // Reallocate memory for characters
        if (!(new_characters = (char*)realloc(result->characters, new_capacity * sizeof(char)))) {
            
            // Failed to reallocate memory for characters
            fprintf(stderr, "Failed to reallocate memory for result data: failed to reallocate memory for characters\n"); 
            return ERROR; 
        }
        
        // Reallocate memory for counts
        if (!(new_counts = (size_t*)realloc(result->counts, new_capacity * sizeof(size_t)))) {
            
            // Failed to reallocate memory for counts
            fprintf(stderr, "Failed to reallocate memory for result data: failed to reallocate memory for counts\n"); 
            free(new_characters);
            return ERROR; 
        }
        
    } else {
        
        // Free the existing arrays
        free(result->characters); 
        free(result->counts); 
    }
    
    // Assign reallocated arrays to fields
    result->characters = new_characters; 
    result->counts = new_counts; 
    result->capacity = new_capacity; 
    
    // Successfully reallocated result data
    return SUCCESS;
}

// EOF