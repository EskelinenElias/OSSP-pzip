#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define NUM_THREADS 4

typedef struct {
    char *input;
    size_t start;
    size_t end;
    char *output;
    size_t *output_size;
} thread_data_t;

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

// Function to open multiple files and concatenate them into a single stream via pipe
FILE* open_files(int argc, char* argv[]) {
    int file_descriptors[2];
    pid_t process_id;
    // Create a pipe
    if (pipe(file_descriptors) == -1) {
        perror("pipe error");
        return NULL;
    }
    // Fork a child process
    process_id = fork();
    if (process_id == -1) {
        perror("fork error");
        return NULL;
    }
    if (process_id == 0) {  // Child process
        close(file_descriptors[0]); // Close read end of the pipe in child
        dup2(file_descriptors[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(file_descriptors[1]); // Close the original write end after redirecting
        // Open each input file and write its content to stdout (pipe)
        for (int i = 0; i < argc; i++) {
            FILE *input = fopen(argv[i], "rb"); // Open in binary mode
            if (input == NULL) {
                fprintf(stderr, "wzip: cannot open file %s\n", argv[i]);
                _exit(1);
            }
            fclose(input);
        }
        return NULL; // Properly exit the child process after writing
    } else {  // Parent process
        close(file_descriptors[1]); // Close write end of the pipe in parent
        wait(NULL); // Wait for the child process to finish
        return fdopen(file_descriptors[0], "rb"); // Open read end as FILE*
    }
}

// Function to read file contents into memory
char* map_file_to_memory(FILE* input) {
    fseek(input, 0, SEEK_END);
    size_t size = ftell(input);
    fseek(input, 0, SEEK_SET);

    char *data = malloc(size);
    if (data) {
        fread(data, 1, size, input);
    }
    return data;
}

// Function to perform RLE compression on a chunk of data
void *compress_chunk(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    size_t output_index = 0;
    char last_char = data->input[data->start];
    int count = 1;

    for (size_t i = data->start + 1; i < data->end; i++) {
        if (data->input[i] == last_char) {
            count++;
        } else {
            data->output[output_index++] = last_char;
            data->output[output_index++] = count;
            last_char = data->input[i];
            count = 1;
        }
    }

    // Handle the last character run in the chunk
    data->output[output_index++] = last_char;
    data->output[output_index++] = count;
    *data->output_size = output_index;
    return NULL;
}

// Function to compress file and feed to output
int compress_data(char* input_data, char* output_buffer) {
    char current_char = input_data[0]; 
    char previous_char = current_char; 
    int length = size(input_data); 
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

int compress_in_parallel(FILE* input, FILE* output) {
    char* input_data = map_file_to_memory(input); 
    size_t input_size = sizeof(input_data); 
    char *output_buffer = malloc(input_size * 2);
    size_t output_sizes[NUM_THREADS];
    // Prepare for parallel RLE compression
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    size_t chunk_size = input_size / NUM_THREADS;
    return 0; 
    // Create threads for parallel compression
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].input = input_data;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? input_size : (i + 1) * chunk_size;
        thread_data[i].output = output_buffer + (i * input_size); // Offset for each thread
        thread_data[i].output_size = &output_sizes[i];
        pthread_create(&threads[i], NULL, compress_chunk, &thread_data[i]);
    }
    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // Write compressed data from each thread to the output file
    for (int i = 0; i < NUM_THREADS; i++) {
        fwrite(thread_data[i].output, 1, output_sizes[i], output);
    }
    // Cleanup
    free(input_data);
    free(output_buffer);
    return 0; // Success
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "wzip: file1 [file2 ...]\n");
        return 1;
    }
    // Open files and concatenate into a single stream
    FILE *input = open_files(argc - 1, &argv[1]);
    // Output concatenated content (for testing purposes)
    concatenate_file(stdout, input); 
    //compress_in_parallel(input, stdout); 
    fclose(input);
    return 0;
}
