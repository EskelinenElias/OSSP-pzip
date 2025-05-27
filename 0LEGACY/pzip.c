#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
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

// Function to process the input in threads
void *worker_thread(void *arg) {
    
    // Cast arguments
    task_queue_t *queue = (task_queue_t *)arg;
    
    // Process tasks as they get added to the task queue
    while (TRUE) {
        
        // Get a task from the queue 
        task_t task = get_task(queue); 
        
        // If the task is a termination task, break 
        if (task.input == NULL) { break; }
        
        // Otherwise, do compression
        int output_length = compress_string(task.input, task.input_length, task.output); 
       
        // Null terminate the output
        task.output[output_length] = '\0';
    }
    return NULL; 
}

// Function to create a new task 
task_t create_task(char* input, char* output, size_t input_length) {
    task_t task; 
    task.input = input; 
    task.input_length = input_length; 
    task.output = output; 
    task.output_length = 0; 
    return task; 
}

// Function to terminate all threads; sends termination signal, then waits for threads to finish
void terminate_threads(task_queue_t* queue, pthread_t* thread, int num_threads) {
    // Send termination signal to all threads
    for (int i = 0; i < num_threads; i++) {
        // Create termination task
        task_t task = create_task(NULL, NULL, CHUNK_SIZE); 
        // Add termination task to the queue
        add_task(queue, task);
    } 
    // Wait for all threads to terminate
    for (int i = 0; i < num_threads; i++) {
        pthread_join(*thread, NULL);
        thread++; 
    }
    return; 
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
