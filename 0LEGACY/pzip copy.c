#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/_pthread/_pthread_t.h>
#include "compress.c"
#include "queue.c"

#define MAX_NUM_THREADS 4
#define CHUNK_SIZE 5
#define TRUE 1
#define FALSE 0

typedef struct {
    char* input; 
    size_t input_length; 
    char* output; 
    size_t output_length; 
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} thread_data_t;

// Function to create a new task 
task_t create_task(char* input, char* output, size_t input_length) {
    task_t task; 
    task.input = input; 
    task.input_length = input_length; 
    task.output = output; 
    task.output_length = 0; 
    return task; 
}

// Compression function; the function distributes the input to threads for compression
void compress_parallel(char* input, int length) {
    
    // Calculate number of chunk and the size of allocated memory
    size_t chunk_output_size = CHUNK_SIZE * (sizeof(int) + sizeof(char));
    size_t num_chunks = ceil((double)strlen(input) / CHUNK_SIZE);
    size_t num_threads = fmin(MAX_NUM_THREADS, num_chunks); 
    

    
    // Create a task queue and output array
    task_queue_t queue = init_queue(num_threads * 2); // Task queue
    char output[num_chunks][chunk_output_size]; // Output array
    
    // Create threads
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    for (int i = 0; i < num_threads; i++) { 
        pthread_create(&threads[i], NULL, worker_thread, &queue); 
    }
    
    // Add tasks to the queue
    printf("Adding tasks...\n"); 
    char* input_ptr = input; // Input pointer
    for (int i = 0; i < num_chunks; i++) {
        
        // Create task
        int lenght = fmin(CHUNK_SIZE, )
        task_t task = create_task(input_ptr, output[i], CHUNK_SIZE); 
        
        // Add task to the queue
        add_task(&queue, task); 
        
        // Advance input pointer
        input_ptr += CHUNK_SIZE; 
    }
    
    // Terminate threads 
    terminate_threads(&queue, &threads[0], num_threads);
    
    // Free memory
    destroy_queue(&queue); 
    
    // Printing output
    for (int i = 0; i < num_chunks; i++) {
        printf("%s\n", output[i]);
    }
    //printf("\n"); 
    return; 
}

int main() {
    char input[] = "aaaaabbbbaaaaaaaaaaaaaaabbbbbaaaaabbbbbbbbaaaa";
    int length = strlen(input) ;
    
    compress_parallel(input, length); 
    return 0; 
}
