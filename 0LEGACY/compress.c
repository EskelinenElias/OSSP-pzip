#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

// Write count and character to the buffer and return updated buffer pointer
inline char* bytes_to_buffer(char* buffer, int count, char character) {
    // Write count as a 4-byte integer
    *(int *)(buffer) = count;
    buffer += sizeof(int); // Advance by 4 bytes

    // Write character as a 1-byte value
    *buffer = character;
    buffer += sizeof(char); // Advance by 1 byte

    // Return the updated buffer pointer
    return buffer; 
}

// Write count and character to the buffer and return updated buffer pointer
char* write_to_buffer(char* buffer, int count, char character) {
    return buffer + sprintf(buffer, "%d%c", count, character); 
}

// Function to compress the string into the output buffer
int compress_string(const char* input_data, size_t length, char* output_buffer) {
    // Initialize variables
    char current_char = input_data[0];
    int count = 1; // Count occurrences of the current character
    char* output_ptr = output_buffer; // Pointer to track buffer position
    length = fmin(length, strlen(input_data)); 
    // Loop through the input data
    for (size_t i = 1; i < length; i++) {
        if (current_char == input_data[i]) {
            // Increment count if the characters match
            count++;
        } else {
            // Write character count to the buffer
            output_ptr = write_to_buffer(output_ptr, count, current_char);
            // Reset for the new character
            current_char = input_data[i];
            count = 1;
        }
    }
    // Write the last character count
    output_ptr = write_to_buffer(output_ptr, count, current_char);
    // Return the total number of bytes written to the output buffer
    return output_ptr - output_buffer;
}