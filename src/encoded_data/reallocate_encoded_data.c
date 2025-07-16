#include "../../include/encoded_data/reallocate_encoded_data.h"

// Function to increase encoded data structure capacity
int reallocate_encoded_data(encoded_data_t* encoded_data, size_t new_capacity) {
    
    // Input validation
    if (encoded_data == NULL || new_capacity < 0) {
        
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
        if (!(new_characters = (char*)realloc(encoded_data->characters, new_capacity * sizeof(char)))) {
            
            // Failed to reallocate memory for characters
            fprintf(stderr, "Failed to reallocate memory for encoded data: failed to reallocate memory for characters\n"); 
            return ERROR; 
        }
        
        // Reallocate memory for counts
        if (!(new_counts = (size_t*)realloc(encoded_data->counts, new_capacity * sizeof(size_t)))) {
            
            // Failed to reallocate memory for counts
            fprintf(stderr, "Failed to reallocate memory for encoded data: failed to reallocate memory for counts\n"); 
            free(new_characters);
            return ERROR; 
        }
        
    } else {
        
        // Free the existing arrays
        free(encoded_data->characters); 
        free(encoded_data->counts); 
    }
    
    // Assign reallocated arrays to fields
    encoded_data->characters = new_characters; 
    encoded_data->counts = new_counts; 
    encoded_data->capacity = new_capacity; 
    
    // Successfully reallocated encoded data
    return SUCCESS;
}

// EOF