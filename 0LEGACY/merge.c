#include <stdio.h>
#include <string.h>
#include <math.h>

// Write count and character to the buffer and return updated buffer pointer
char* write_to_buffer(char* buffer, int count, char character) {
    return buffer + sprintf(buffer, "%d%c", count, character); 
}

// Write count and character to the buffer and return updated buffer pointer
char* bytes_to_buffer(char* buffer, int count, char character) {
    // Write count as a 4-byte integer
    *(int *)(buffer) = count;
    buffer += sizeof(int); // Advance by 4 bytes

    // Write character as a 1-byte value
    *buffer = character;
    buffer += sizeof(char); // Advance by 1 byte

    // Return the updated buffer pointer
    return buffer; 
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
            output_ptr = bytes_to_buffer(output_ptr, count, current_char);
            // Reset for the new character
            current_char = input_data[i];
            count = 1;
        }
    }
    // Write the last character count
    output_ptr = bytes_to_buffer(output_ptr, count, current_char);
    // Return the total number of bytes written to the output buffer
    return output_ptr - output_buffer;
}

int main() {
    char str1[] = "aaaabbbbbbbbbbb"; 
    size_t str1len = strlen(str1); 
    
    char str2[] = "bbaaaaaccc"; 
    size_t str2len = strlen(str2); 
    
    char rle1[str1len*(sizeof(int) + sizeof(char))]; 
    char rle2[str2len*(sizeof(int) + sizeof(char))]; 

    size_t rle1len = compress_string(str1, str1len, rle1);  
    size_t rle2len = compress_string(str2, str2len, rle2); 

    printf("rle len %lu\n", rle1len); 
    
    for (int i = 0; i < rle1len; i++) {
        printf("%c\n", rle1[i]); 
    }
    
    printf("%\n", rle1); 
}