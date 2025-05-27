#include "../include/output.h"

// Function to write compressed data to output as bytes
char* write_data_to_buffer(char* buffer, encoded_data_t data) {
    
    for (int i = 0; i < data.size; i++) {
        
        // Write count as a 4-byte integer
        *(int *)(buffer) = data.counts[i];
        buffer += sizeof(int); // Advance by 4 bytes
    
        // Write character as a 1-byte value
        *buffer = data.characters[i];
        buffer += sizeof(char); // Advance by 1 byte
    }
    
    // Return the updated buffer pointer
    return buffer; 
}

// Function to write compressed data to buffer as text (for testing purposes)
void write_text_to_buffer(FILE* buffer, encoded_data_t data) {
        
    // Process the data
    for (int i = 0; i < data.size; i++) {
                
        // Write count and character to buffer and advance buffer
        fprintf(stdout, "%c%lu", data.characters[i], data.counts[i]); 
    }
}