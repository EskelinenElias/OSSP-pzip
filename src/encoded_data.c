#include "../include/encoded_data.h"

// Function to create and initialize a new RLE data array
encoded_data_t* create_encoded_data(size_t initial_capacity) {
    
    // Allocate memory for the encoded data structure
    encoded_data_t* encoded_data = (encoded_data_t*)malloc(sizeof(encoded_data_t));
    if (encoded_data == NULL) {
        fprintf(stderr, "create_encoded_data: Memory allocation failed!\n");
        return NULL;
    }    

    // Initialize fields
    encoded_data->characters = (char*)malloc(initial_capacity * sizeof(char));
    encoded_data->counts = (size_t*)malloc(initial_capacity * sizeof(size_t));
    encoded_data->size = 0;
    encoded_data->capacity = initial_capacity; 
    
    // Check that memory allocation was successful
    if (encoded_data->characters == NULL || encoded_data->counts == NULL) {
        fprintf(stderr, "create_encoded_data: Memory allocation failed!\n");
        exit(1); // Error
    } 
    
    return encoded_data; // Success
}

// Function to increase RLE data array capacity
int realloc_encoded_data(encoded_data_t* encoded_data, size_t new_capacity) {
    
    // Input validation
    if (encoded_data == NULL || new_capacity <= encoded_data->capacity) {
        fprintf(stderr, "realloc_encoded_data: Invalid input.\n"); 
        return FAILURE; // Error
    }
    
    // Reallocate memory
    char* new_characters = (char*)realloc(encoded_data->characters, new_capacity * sizeof(char));
    size_t* new_counts = (size_t*)realloc(encoded_data->counts, new_capacity * sizeof(size_t));
    
    // Check that reallocation was successful
    if (new_characters == NULL || new_counts == NULL) {
        fprintf(stderr, "realloc_encoded_data: Memory reallocation failed.\n"); 

        // Free allocated arrays
        if (new_characters != NULL) free(new_characters);
        if (new_counts != NULL) free(new_counts);
        
        return FAILURE; // Error
    }
    
    // Assign reallocated arrays to fields
    encoded_data->characters = new_characters; 
    encoded_data->counts = new_counts; 
    encoded_data->capacity = new_capacity; 
    
    return SUCCESS; // Success
}

// Function to free the memory allocated for a RLE data array
void free_encoded_data(encoded_data_t* encoded_data) {
    
    // Input validation
    if (!encoded_data) {
        return; 
    }
        
    // Free characters array
    if (encoded_data->characters) {
        free(encoded_data->characters);
        encoded_data->characters = NULL; 
    }
        
    // Free counts array
    if (encoded_data->counts) {
        free(encoded_data->counts);
        encoded_data->counts = NULL;   
    }

    // Reset size and capacity
    encoded_data->size = 0;
    encoded_data->capacity = 0;

    // Free the encoded data structure
    free(encoded_data); 
}

// Function to write to the RLE data array and dynamically reallocate memory if necessary
int append_to_encoded_data(encoded_data_t* encoded_data, char character, size_t count) {
    
    // Input validation 
    if (encoded_data == NULL) {
        fprintf(stderr, "append_to_encoded_data: Invalid input.\n");
        return FAILURE; // Error
    }

    // Reallocate memory if necessary 
    if (encoded_data->size >= encoded_data->capacity) {
        size_t new_capacity = encoded_data->capacity + INIT_CAPACITY; 
        if (realloc_encoded_data(encoded_data, new_capacity) != SUCCESS) {
            fprintf(stderr, "write_to_encoded_data_dynamically: Failed to reallocate memory!\n");
            // Check if memory was deallocated successfully
            if (encoded_data->characters != NULL || encoded_data->counts != NULL) {
                free_encoded_data(encoded_data);
            }
            return FAILURE; // Error
        }
    }
    
    // Write character count to current index
    size_t index = encoded_data->size; 
    encoded_data->characters[index] = character; 
    encoded_data->counts[index] = count; 
    encoded_data->size++; // Increase size
    return SUCCESS; // Success
}