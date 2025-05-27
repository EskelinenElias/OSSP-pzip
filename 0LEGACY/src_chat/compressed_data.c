#include "../include/constants.h"
#include "../include/compressed_data.h"

// Function to create and initialize a new RLE data array
rle_data_t create_rle_data(size_t initial_capacity) {
    
    // Initialize object
    rle_data_t rle_data;
    
    // Initialize fields
    rle_data.characters = (char*)malloc(initial_capacity * sizeof(char));
    rle_data.counts = (size_t*)malloc(initial_capacity * sizeof(size_t));
    rle_data.size = 0;
    rle_data.capacity = initial_capacity; 
    
    // Check that memory allocation was successful
    if (rle_data.characters == NULL || rle_data.counts == NULL) {
        fprintf(stderr, "create_rle_data: Memory allocation failed!\n");
        exit(1); // Error
    } 
    
    return rle_data; // Success
}

// Function to increase RLE data array capacity
int realloc_rle_data(rle_data_t* rle_data, size_t new_capacity) {
    
    // Input validation
    if (rle_data == NULL || new_capacity <= rle_data->capacity) {
        fprintf(stderr, "realloc_rle_data: Invalid input.\n"); 
        return FAILURE; // Error
    }
    
    // Reallocate memory
    char* new_characters = (char*)realloc(rle_data->characters, new_capacity * sizeof(char));
    size_t* new_counts = (size_t*)realloc(rle_data->counts, new_capacity * sizeof(size_t));
    
    // Check that reallocation was successful
    if (new_characters == NULL || new_counts == NULL) {
        fprintf(stderr, "realloc_rle_data: Memory reallocation failed.\n"); 

        // Free allocated arrays
        if (new_characters != NULL) free(new_characters);
        if (new_counts != NULL) free(new_counts);
        
        return FAILURE; // Error
    }
    
    // Assign reallocated arrays to fields
    rle_data->characters = new_characters; 
    rle_data->counts = new_counts; 
    rle_data->capacity = new_capacity; 
    
    return SUCCESS; // Success
}

// Function to free the memory allocated for a RLE data array
void free_rle_data(rle_data_t* rle_data) {
    if (rle_data != NULL) {
        
        // Free characters array
        if (rle_data->characters != NULL) {
            free(rle_data->characters);
            rle_data->characters = NULL; 
        }
        
        // Free counts array
        if (rle_data->counts != NULL) {
            free(rle_data->counts);
            rle_data->counts = NULL;   
        }
        
        // Set other fields to 0
        rle_data->size = 0;
        rle_data->capacity = 0;
    }
}

// Function to write to the RLE data array and dynamically reallocate memory if necessary
int append_to_rle_data(rle_data_t* rle_data, char character, size_t count) {
    
    // Input validation 
    if (rle_data == NULL) {
        fprintf(stderr, "append_to_rle_data: Invalid input.\n");
        return FAILURE; // Error
    }

    // Reallocate memory if necessary 
    if (rle_data->size >= rle_data->capacity) {
        size_t new_capacity = rle_data->capacity + INIT_CAPACITY; 
        if (realloc_rle_data(rle_data, new_capacity) != SUCCESS) {
            fprintf(stderr, "write_to_rle_data_dynamically: Failed to reallocate memory!\n");
            // Check if memory was deallocated successfully
            if (rle_data->characters != NULL || rle_data->counts != NULL) {
                free_rle_data(rle_data);
            }
            return FAILURE; // Error
        }
    }
    
    // Write character count to current index
    size_t index = rle_data->size; 
    rle_data->characters[index] = character; 
    rle_data->counts[index] = count; 
    rle_data->size++; // Increase size
    return SUCCESS; // Success
}