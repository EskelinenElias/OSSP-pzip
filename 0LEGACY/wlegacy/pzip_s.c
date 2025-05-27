#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

typedef struct {
    char *input; // Pointer to input data chunk (memory-mapped)
    size_t start; // Start index for this chunk
    size_t end; // End index for this chunk
    char *output; // Output buffer for the compressed data
    size_t *output_size; // Pointer to output size (for each thread)
} Chunk;

// Function to write contents of input2 to the end of input1
int concatenate_file(FILE* output, FILE* input) {
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    // Read from input file until EOF and write to output
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        fwrite(buffer, 1, bytes_read, output);
    }
    return 0;
}

// Function to compress file and feed to output
int compress_file(FILE* input, FILE* output) {
    char current_char = fgetc(input); 
    char previous_char = current_char; 
    int count = 1; 
    
    // Check if the file is empty
    if (current_char == EOF) { return 0; }
    
    // Read input character by character until end of file
    while ((current_char = fgetc(input)) != EOF) {
        if (current_char == previous_char) {
            count++;
        } else {
            // Write the character and the count to output
            fwrite(&count, sizeof(int), 1, stdout); 
            fwrite(&previous_char, sizeof(char), 1, output);
            // Reset count and set previous character to the current character
            count = 1; 
            previous_char = current_char; 
        }
    }
    // Write the last character and the count to output
    fwrite(&count, sizeof(int), 1, output); 
    fwrite(&previous_char, sizeof(char), 1, output);
    return 0; 
} 

// Function to compress input file using RLE compression
int compress_parallel(int fd_input) {
    
    return 0;
}

// Function to compress a chunk 
int compress_chunk(Chunk* chunk) {
    
    return 0; 
}

// Main function parses the arguments
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "wzip: file1 [file2 ...]\n");
        return 1;
    }
    // Open files and concatenate into a single stream
    FILE *input = fopen(argv[1], "r");
    // Output concatenated content (for testing purposes)
    concatenate_file(stdout, input); 
    //compress_in_parallel(input, stdout); 
    fclose(input);
    return 0;
}