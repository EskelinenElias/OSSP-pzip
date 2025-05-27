#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define MAX_NUM_THREADS 4
#define CHUNK_SIZE 10
#define TRUE 1
#define FALSE 0

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

char* compress_loop(char* input) {
    // Calculate number of chunk and the size of allocated memory
    size_t chunk_output_size = CHUNK_SIZE * (sizeof(int)+sizeof(char));
    size_t num_chunks = ceil((double)strlen(input) / CHUNK_SIZE);
    printf("CHUNKS: %lu\n", num_chunks); 
    // Initialize output queue (an array of strings)
    char output_queue[num_chunks][chunk_output_size];
    int output_length[num_chunks];
    // Create a pointer to the input string
    char* input_ptr = input; 
    // Compress the input in chunks
    for (int i = 0; i < num_chunks; i++) {
        // Compress the chunk
        output_length[i] = compress_string(input_ptr, CHUNK_SIZE, output_queue[i]);
        // Advance the input pointer
        input_ptr += CHUNK_SIZE; 
    }
    // Print the output to standard output
    char string[CHUNK_SIZE];
    for (int i = 0; i < num_chunks; i++) {
        strncpy(string, output_queue[i], CHUNK_SIZE); 
        printf("%s\n", string);
    }
    return 0; 
}

typedef struct {
    int is_free; 
    char* input; 
    size_t input_length; 
    char* output; 
    size_t output_length; 
} thread_data_t;

void *worker(void *args) {
    thread_data_t *data = (thread_data_t *)args; 
    while (TRUE) {
        if (!data->is_free) {
            data->output_length = compress_string(data->input, data->input_length, data->output); 
            data->is_free = TRUE; 
        }
    }
}

void compress_parallel(char* input) {
    // Calculate number of chunk and the size of allocated memory
    size_t chunk_output_size = CHUNK_SIZE * (sizeof(int) + sizeof(char));
    size_t num_chunks = ceil((double)strlen(input) / CHUNK_SIZE);
    printf("CHUNKS: %lu\n", num_chunks); 
    // Prepare for parallel RLE compression
    size_t num_threads = fmin(MAX_NUM_THREADS, num_chunks); 
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    // Initialize output queue (an array of strings)
    char output_queue[num_chunks][chunk_output_size];
    // Create a pointer to the input string
    char* input_ptr = input; 
    size_t output_index = 0; 
    // Create threads for parallel compression
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].is_free = TRUE; 
        thread_data[i].input = NULL; 
        thread_data[i].input_length = CHUNK_SIZE; 
        thread_data[i].output = NULL; 
        thread_data[i].output_length = 0; 
        pthread_create(&threads[i], NULL, worker, &thread_data[i]);
        input_ptr += CHUNK_SIZE; 
    }
    // Wait for threads to complete
    for (int j = 0; j < num_chunks; j++) {
        for (int i = 0; i < num_threads; i++) {
            if (thread_data[i].is_free) {
                thread_data[i].is_free = FALSE; 
                thread_data[i].input = input_ptr;
                thread_data[i].input_length = CHUNK_SIZE; 
                thread_data[i].output = output_queue[output_index++]; 
                break; 
            }
        }
        input_ptr += CHUNK_SIZE; 
    }
    // Join all threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    char input[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    compress_parallel(input); 
    return 0; 
}
