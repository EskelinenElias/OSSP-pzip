#include "../../include/encoded_data/init_encoded_data.h"

// Function to initialize encoded data
encoded_data_t* init_encoded_data(size_t initial_capacity) {
    
    // Allocate memory for encoded data
    encoded_data_t* encoded_data = malloc(sizeof(encoded_data_t));
    if (!encoded_data) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize encoded data: failed to allocate memory for encoded_data\n");
        return NULL;
    }    
    
    // Set encoded data fields
    encoded_data->characters = NULL; 
    encoded_data->counts = NULL; 
    encoded_data->capacity = initial_capacity;
    
    // Check if initial capacity is greater than 0
    if (initial_capacity > 0) {

        // Allocate memory for characters array
        if (!(encoded_data->characters = calloc(initial_capacity, sizeof(char)))) {
            
            // Failed to allocate memory for characters array
            fprintf(stderr, "Failed to initialize encoded data: failed to allocate memory for characters array\n");
            free(encoded_data);
            return NULL;
        }
        
        // Allocate memory for counts array
        if (!(encoded_data->counts = calloc(initial_capacity, sizeof(size_t)))) {
            
            // Failed to allocate memory for counts array
            fprintf(stderr, "Failed to initialize encoded data: failed to allocate memory for counts array\n");
            free(encoded_data->characters);
            free(encoded_data);
            return NULL;
        }
    }
    
    // Successfully initialized encoded data
    return encoded_data; 
}

// EOF