#include "../../include/result_data/init_result_data.h"

// Function to initialize result data
result_data_t* init_result_data(size_t initial_capacity) {
    
    // Allocate memory for the encoded data structure
    result_data_t* result = (result_data_t*)malloc(sizeof(result_data_t));
    if (!result) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize result data: failed to allocate memory for result\n");
        return NULL;
    }    
    
    // Set result fields
    result->characters = NULL; 
    result->counts = NULL; 
    result->capacity = initial_capacity;
    
    // Check if initial capacity is greater than 0
    if (initial_capacity > 0) {

        // Allocate memory for characters
        if (!(result->characters = (char*)malloc(initial_capacity * sizeof(char)))) {
            
            // Failed to allocate memory
            fprintf(stderr, "Failed to initialize result data: failed to allocate memory for characters\n");
            free(result);
            return NULL;
        }
        
        // Allocate memory for counts
        if (!(result->counts = (size_t*)malloc(initial_capacity * sizeof(size_t)))) {
            
            // Failed to allocate memory
            fprintf(stderr, "Failed to initialize result data: failed to allocate memory for counts\n");
            free(result->characters);
            free(result);
            return NULL;
        }
    }
    
    // Successfully initialized result data
    return result; 
}

// EOF