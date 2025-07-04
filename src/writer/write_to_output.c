#include "../../include/writer/write_to_output.h"

const int TEST = FALSE; 

// Function to write encoded data to standard output as bytes
int write_encoded_data_to_output(result_data_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Failed to write result to output. Invalid input\n");
        return ERROR; 
    }
    
    // Check if the test flag is enabled
    if (TEST) {
        
        // Write test data to output as text
        write_encoded_text_to_output(result); 
        return SUCCESS; 
    }

    // Iterate through the encoded data
    for (int i = 0; i < result->capacity; i++) {
        
        // Write count and character to output
        fwrite(&result->counts[i], sizeof(int), 1, stdout); 
        fwrite(&result->characters[i], sizeof(char), 1, stdout);    
    }
    
    // Return the updated buffer pointer
    return SUCCESS; 
}

// Function to write encoded data to standard output as text (for testing purposes)
int write_encoded_text_to_output(result_data_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Failed to write result to output. Invalid input\n");
        return ERROR; 
    }

    // Process the data
    for (int i = 0; i < result->capacity; i++) {
                
        // Write count and character to buffer and advance buffer
        fprintf(stdout, "%lu%c", result->counts[i], result->characters[i]); 
    }

    return SUCCESS; 
}