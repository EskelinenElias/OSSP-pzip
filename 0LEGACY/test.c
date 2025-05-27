#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/constants.h"
#define INIT_CAPACITY 16

typedef struct {
    char* characters;
    size_t* counts;
    size_t size;
    size_t capacity;
} rle_data_t;

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

// Function to count run lengths for a input string
int count_run_lengths(char* input, size_t length, rle_data_t* output) {
    
    // Input validation 
    if (input == NULL || length <= 0 || output == NULL) {
        
        // Invalid input
        fprintf(stderr, "count_run_lengths: Invalid input.\n");
        return FAILURE; // Error
    }
    
    // Initialize the tracked character to the first character of the input and set count to 1
    char tracked_char = input[0];
    size_t tracked_count = 1;
    
    // Count run lenghts of subsequent matching characters in the input
    for (size_t i = 1; i < length; i++) {
        
        // Check if the tracked character is the same as the current character in the input
        if (tracked_char == input[i]) {
            
            // Increment count if the characters match
            tracked_count++;
        
        } else {
            
            // Append the tracked character and count to the output
            if (append_to_rle_data(output, tracked_char, tracked_count) != SUCCESS) {
                return FAILURE; // Error
            }
            
            // Set the tracked character to the current character in the input and set count to 1
            tracked_char = input[i];
            tracked_count = 1;
        }
    }

    // Append the tracked character and count to the output
    if (append_to_rle_data(output, tracked_char, tracked_count) != SUCCESS) {
        return FAILURE; 
    };
    
    return SUCCESS; // Success
}

// Main function 
int main() {
    
    char input[] = "aaaaaaaabbbbbbbbaaaabbbaaabbaaaaaababaaaaabaaabbbbbbbaaaababaaabababbbbbaaaabaaababbbbaaaabaabaababbbbbababaabaabbabbbaaaaaaabbbbbaaaababbababbbbaaabababbbbbbaaaaaabababababbbbbbbbbbaaabbba"; 
    //char input[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"; 
    rle_data_t output = create_rle_data(16); 
    int result = count_run_lengths(&input[0], strlen(input), &output);
    
    if (result == 1) {
        fprintf(stdout, "Failed\n"); 
        exit(1); 
    }   
    
    for (int i = 0; i < output.size; i++) {
        
        fprintf(stdout, "%c%lu", output.characters[i], output.counts[i]); 

    }
    
    fprintf(stdout, "\nAll done.\n"); 

}   