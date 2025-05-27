#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Buffer size for reading lines
#define BUFFER_SIZE 1024

// Function to combine files 
int concatenate_to_output(FILE* input1, FILE* input2, FILE* output) {
    char buffer[BUFFER_SIZE];
    // Read input 1 line by line until end of file
    while (fgets(buffer, BUFFER_SIZE, input1) != NULL) {
        // Write each line in input 1 to the end of output
        fputs(buffer, output);
    } 
    // Read input 2 line by line until end of file
    while (fgets(buffer, BUFFER_SIZE, input2) != NULL) {
        // Write each line in input 2 to the end of output
        fputs(buffer, output);
    } 
    return 0; 
}

// Function to write contents of input 2 to the end of input 1
int concatenate(FILE* input1, FILE* input2) {
    char buffer[BUFFER_SIZE];
    // Read input 2 until end is reached
    while (fgets(buffer, BUFFER_SIZE, input2) != NULL) {
        // Write contents to the end of input 1
        fputs(buffer, input1);
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

            // Increment the count for the current character
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

int main(int argc, char *argv[]) {

    // Check that the correct number of arguments were provided
    if (argc < 2) {
        fprintf(stdout, "pzip: file1 [file2 ...]\n");
        return 1;
    }
    
    // Prepare variables for pipe and fork
    int file_descriptors[2];
    pid_t process_id;
    
    // Create a pipe
    if (pipe(file_descriptors) == -1) {
        perror("pipe");
        return 1;
    }
    
    // Fork a child process
    process_id = fork();
    if (process_id == -1) {
        perror("fork");
        return 1;
    }
    
    // Execute the next steps according to the current process 
    if (process_id == 0) {  // Child process

        // Close the read end of the pipe, as the child will write to it
        close(file_descriptors[0]);

        // Redirect standard output to the write end of the pipe
        dup2(file_descriptors[1], STDOUT_FILENO);

        // Close the original write end after redirecting
        close(file_descriptors[1]); 

        // Read input files and write their contents to the pipe 
        for (int i = 1; i < argc; i++) {

            // Open input file
            FILE *input = fopen(argv[i], "r");

            // Check that the file opened successfully
            if (input == NULL) {
                fprintf(stderr, "wzip: cannot open file\n");
                return 1;
            }

            // Concatenate file contents to the end of standard output (pipe)
            concatenate(stdout, input);

            // Close the input file
            fclose(input);
        }
        // Exit child process after writing all files
        return 0;

    } else {  // Parent process

        // Close the write end of the pipe
        close(file_descriptors[1]);

        // Open the read end of the pipe as a input stream
        FILE *input = fdopen(file_descriptors[0], "r");

        // Compress the stream read from the pipe and write to stdout
        compress_file(input, stdout); 

        // Close the pipe stream and wait for the child process to finish
        fclose(input);

        // Wait for the child process to complete
        wait(NULL);  
    }
    return 0;
}