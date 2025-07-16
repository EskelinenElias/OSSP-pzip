#include "../../include/encoding_result/init_encoding_result.h"

// Function to initialize encoded data
encoding_result_t* init_encoding_result(size_t initial_capacity) {
    
    // Allocate memory for encoded data
    encoding_result_t* encoding_result = malloc(sizeof(encoding_result_t));
    if (!encoding_result) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize encoded data: failed to allocate memory for encoding_result\n");
        return NULL;
    }    
    
    // Set encoded data fields
    encoding_result->characters = NULL; 
    encoding_result->counts = NULL; 
    encoding_result->capacity = initial_capacity;
    
    // Check if initial capacity is greater than 0
    if (initial_capacity > 0) {

        // Allocate memory for characters array
        if (!(encoding_result->characters = calloc(initial_capacity, sizeof(char)))) {
            
            // Failed to allocate memory for characters array
            fprintf(stderr, "Failed to initialize encoded data: failed to allocate memory for characters array\n");
            free(encoding_result);
            return NULL;
        }
        
        // Allocate memory for counts array
        if (!(encoding_result->counts = calloc(initial_capacity, sizeof(size_t)))) {
            
            // Failed to allocate memory for counts array
            fprintf(stderr, "Failed to initialize encoded data: failed to allocate memory for counts array\n");
            free(encoding_result->characters);
            free(encoding_result);
            return NULL;
        }
    }
    
    // Successfully initialized encoded data
    return encoding_result; 
}

// EOF