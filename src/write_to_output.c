#include "../include/write_to_output.h"

// Function to write compressed data to output as bytes
int write_encoded_data_to_output(FILE* output, encoded_data_t* data) {
    
    // Input validation
    if (output == NULL || data->size == 0 || data == NULL) {
        return SUCCESS; // Error
    }

    // Iterate through the encoded data
    for (int i = 0; i < data->size; i++) {
        
        // Write count and character to output
        fwrite(&data->counts[i], sizeof(int), 1, output); 
        fwrite(&data->characters[i], sizeof(char), 1, output);    
    }
    
    // Return the updated buffer pointer
    return SUCCESS; 
}

// Function to write compressed data to buffer as text (for testing purposes)
int write_encoded_text_to_output(FILE* output, encoded_data_t* data) {
    
    // Input validation
    if (output == NULL || data->size == 0 || data == NULL) {
        return SUCCESS; // Error
    }

    // Process the data
    for (int i = 0; i < data->size; i++) {
                
        // Write count and character to buffer and advance buffer
        fprintf(output, "%lu%c", data->counts[i], data->characters[i]); 
    }

    return SUCCESS; 
}