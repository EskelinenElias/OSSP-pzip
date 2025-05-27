#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "compress.c"

#define MAX_NUM_THREADS 4
#define CHUNK_SIZE 10
#define TRUE 1
#define FALSE 0

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
    int is_busy; 
    char* input; 
    size_t input_length; 
    char* output; 
    size_t output_length; 
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} thread_data_t;

void* worker_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    
    while (TRUE) {
        // Acquire lock
        pthread_mutex_lock(&data->mutex);
        // Wait until a task is assigned to the worker thread
        pthread_cond_wait(&data->cond, &data->mutex);
        // Signal that the thread is busy
        data->is_busy = TRUE; 
        // Perform the compression task
        data->output_length = compress_string(data->input, data->input_length, data->output);
        // Mark the thread as free and signal the main thread that the task is complete
        data->is_busy = FALSE;
        pthread_cond_signal(&data->cond);  // Notify main thread that the worker is available
        pthread_mutex_unlock(&data->mutex);
    }
    return NULL; 
}

void create_thread(pthread_t* thread, thread_data_t* data) {
    data->is_busy = FALSE; 
    data->input = NULL; 
    data->input_length = CHUNK_SIZE; 
    data->output = NULL; 
    data->output_length = 0;
    pthread_mutex_init(&data->mutex, NULL); 
    pthread_cond_init(&data->cond, NULL); 
    pthread_create(thread, NULL, worker_thread, data); 
}

void assign_to_thread(thread_data_t* data, char* input, int length, char* output) {
    data->input = input; 
    data->input_length = length; 
    data->output = output; 
    data->output_length = 0; 
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
    // Create threads 
    for (int i = 0; i < num_threads; i++) { 
        create_thread(&threads[i], &thread_data[i]); 
    }
    // Wait for threads to complete
    size_t chunk = 0; 
    while (chunk < num_chunks) {
        for (int i = 0; i < num_threads; i++) {
            pthread_mutex_lock(&thread_data[i].mutex); 
            if (!thread_data[i].is_busy) {
                // Assign input to the thread
                thread_data[i].input = input_ptr; 
                // Advance the input pointer
                input_ptr += CHUNK_SIZE; 
                // Assign output slot to the thread
                thread_data[i].output = output_queue[chunk++]; 
                // Signal to the thread that it should start working
                pthread_cond_signal(&thread_data[i].cond);
                pthread_mutex_unlock(&thread_data[i].mutex);
            }
            pthread_mutex_unlock(&thread_data[i].mutex); 
        }
        
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
