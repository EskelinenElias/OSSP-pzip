#include "../include/result.h"

// Function to initialize a new result structure
result_t* init_result() {
    
    // Allocate memory for the result structure
    result_t* result = (result_t*)malloc(sizeof(result_t));
    if (!result) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize result: failed to allocate memory for result\n");
        return NULL;
    }    
    
    // Set result fields
    result->data = NULL; 
    result->status = PENDING; 
    
    // Successfully created result structure
    return result; 
}

// Function to free a result structure
void* free_result(result_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Failed to free result: invalid input\n");
        return NULL;
    }
    
    // Free result data structure
    if (result->data) free_result_data(result->data);
    
    // Free result structure
    free(result);
    
    // Successfully freed result structure
    return NULL;
}; 

// Function to initialize a new result data structure
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
    
    // Check if capacity if 0; this is allowed, as empty result acts as EOF
    if (initial_capacity == 0) return result; 

    // Allocate memory for characters
    result->characters = (char*)malloc(initial_capacity * sizeof(char));
    if (!result->characters) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize result data: failed to allocate memory for characters\n");
        free(result);
        return NULL;
    }
    
    // Allocate memory for counts
    result->counts = (size_t*)malloc(initial_capacity * sizeof(size_t));
    if (!result->counts) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize result data: failed to allocate memory for counts\n");
        free(result->characters);
        free(result);
        return NULL;
    }
    
    // Successfully created result data structure
    return result; 
}

// Function to increase result data structure capacity
int reallocate_result_data(result_data_t* result, size_t new_capacity) {
    
    // Input validation
    if (result == NULL || new_capacity < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to reallocate memory for result data: invalid input\n"); 
        return ERROR; 
    }
    
    // Reallocate memory for characters
    char* new_characters = (char*)realloc(result->characters, new_capacity * sizeof(char));
    if (!new_characters) {
        
        // Failed to reallocate memory for characters
        fprintf(stderr, "Failed to reallocate memory for result data: failed to reallocate memory for characters\n"); 
        return ERROR; 
    }
    
    // Reallocate memory for counts
    size_t* new_counts = (size_t*)realloc(result->counts, new_capacity * sizeof(size_t));
    if (!new_counts) {
        
        // Failed to reallocate memory for counts
        fprintf(stderr, "Failed to reallocate memory for result data: failed to reallocate memory for counts\n"); 
        free(new_characters);
        return ERROR; 
    }
    
    // Assign reallocated arrays to fields
    result->characters = new_characters; 
    result->counts = new_counts; 
    result->capacity = new_capacity; 
    
    // Successfully reallocated memory for result
    return SUCCESS;
}

// Function to free the memory allocated for a result data structure
void* free_result_data(result_data_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Failed to free result data: invalid input\n"); 
        return NULL; 
    }
        
    // Free characters array
    if (result->characters) free(result->characters);
        
    // Free counts array
    if (result->counts) free(result->counts);

    // Free the data structure
    free(result); 
    
    // Successfully freed result
    return NULL; 
}