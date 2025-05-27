#include "../include/read_files.h"

// Function that reads data from a file into a buffer until EOF or the buffer is full
size_t read_to_buffer(FILE *input, char *buffer_start, char *buffer_end) {
    
    // Validate input parameters
    if (!input || !buffer_start || !buffer_end || buffer_start >= buffer_end) {
        return (size_t)-1;
    }

    // Initialize variables
    size_t total_read = 0;
    size_t bytes_read;
    size_t space_left = buffer_end - buffer_start;
    
    // Read until EOF, error, or buffer is full
    while (!feof(input) && !ferror(input) && space_left > 0) {
        bytes_read = fread(buffer_start, 1, space_left, input);
        if (bytes_read == 0) break;  // End of file or error
        buffer_start += bytes_read;
        total_read += bytes_read;
        space_left -= bytes_read;
    }

    if (ferror(input)) {
        perror("Error reading file");
        return (size_t)-1;
    }

    return total_read;
}