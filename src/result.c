#include "../include/result.h"

// Function to initialize a new result data structure
result_t* init_result(size_t initial_capacity) {
    
    // Allocate memory for the encoded data structure
    result_t* result = (result_t*)malloc(sizeof(result_t));
    if (!result) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to allocate memory for result\n");
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
        fprintf(stderr, "Failed to allocate memory for characters\n");
        free(result);
        return NULL;
    }
    
    // Allocate memory for counts
    result->counts = (size_t*)malloc(initial_capacity * sizeof(size_t));
    if (!result->counts) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to allocate memory for counts\n");
        free(result->characters);
        free(result);
        return NULL;
    }
    
    // Successfully created result data structure
    return result; 
}

// Function to increase result data structure capacity
int reallocate_result(result_t* result, size_t new_capacity) {
    
    // Input validation
    if (result == NULL || new_capacity < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to reallocate memory for result. Invalid input\n"); 
        return ERROR; 
    }
    
    // Reallocate memory for characters
    char* new_characters = (char*)realloc(result->characters, new_capacity * sizeof(char));
    if (!new_characters) {
        
        // Failed to reallocate memory for characters
        fprintf(stderr, "Failed to reallocate memory for result characters\n"); 
        return ERROR; 
    }
    
    // Reallocate memory for counts
    size_t* new_counts = (size_t*)realloc(result->counts, new_capacity * sizeof(size_t));
    if (!new_counts) {
        
        // Failed to reallocate memory for counts
        fprintf(stderr, "Failed to reallocate memory for result counts\n"); 
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
result_t* free_result(result_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Cannot free result, result is NULL\n"); 
        return NULL; 
    }
        
    // Free characters array
    if (result->characters) free(result->characters);
        
    // Free counts array
    if (result->counts) free(result->counts);

    // Free the data structure
    free(result); 
    
    // Set the pointer to NULL to avoid dangling pointer
    result = NULL;
    
    // Successfully freed result
    return NULL; 
}

// // Function to write to the RLE data array and dynamically reallocate memory if necessary
// int append_to_encoded_data(result_t* result, char character, size_t count) {
    
//     // Input validation 
//     if (!result) {
        
//         // Invalid input
//         fprintf(stderr, "Failed to append to result. Invalid input\n");
//         return ERROR; // Error
//     }

//     // Reallocate memory if necessary 
//     if (result->capacity >= encoded_data->capacity) {
//         size_t new_capacity = encoded_data->capacity + INIT_CAPACITY; 
//         if (realloc_encoded_data(encoded_data, new_capacity) != SUCCESS) {
//             fprintf(stderr, "write_to_encoded_data_dynamically: Failed to reallocate memory!\n");
//             // Check if memory was deallocated successfully
//             if (encoded_data->characters != NULL || encoded_data->counts != NULL) {
//                 free_encoded_data(encoded_data);
//             }
//             return FAILURE; // Error
//         }
//     }
    
//     // Write character count to current index
//     size_t index = encoded_data->size; 
//     encoded_data->characters[index] = character; 
//     encoded_data->counts[index] = count; 
//     encoded_data->size++; // Increase size
//     return SUCCESS; // Success
// }