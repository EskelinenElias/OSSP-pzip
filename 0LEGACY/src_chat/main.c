#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/constants.h"
#include "../include/task_queue.h"
#include "../include/thread_worker.h"
#include "../include/rle_compress.h"

int child(int file_descriptors[], int argc, char *argv[]) {

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
}


// Main function 
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

        // Run child process
        return child(file_descriptors, argc, argv);

    } else {  // Parent process

        // Close the write end of the pipe
        close(file_descriptors[1]);

        // Open the read end of the pipe as a input stream
        FILE *input = fdopen(file_descriptors[0], "r");

        // Open the output stream
        FILE *output = stdout;

        // Compress the stream read from the pipe and write to stdout
        parallel_RLE(input, output); 

        // Close the pipe stream and wait for the child process to finish
        fclose(input);

        // Wait for the child process to complete
        wait(NULL);  
    }
    return 0;
}