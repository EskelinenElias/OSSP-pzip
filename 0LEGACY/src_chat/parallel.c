#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "../include/constants.h"
#include "../include/task_queue.h"
#include "../include/thread_worker.h"
#include "../include/rle_compress.h"

// Function to write compressed data to output
char* write_data_to_buffer(char* buffer, rle_data_t data) {
    
    for (int i = 0; i < data.size; i++) {
        
        // Write count as a 4-byte integer
        *(int *)(buffer) = data.counts[i];
        buffer += sizeof(int); // Advance by 4 bytes
    
        // Write character as a 1-byte value
        *buffer = data.characters[i];
        buffer += sizeof(char); // Advance by 1 byte
    }
    
    // Return the updated buffer pointer
    return buffer; 
}

// Function to write compressed data to buffer as text (for testing purposes)
void write_text_to_buffer(FILE* buffer, rle_data_t data) {
        
    // Process the data
    for (int i = 0; i < data.size; i++) {
                
        // Write count and character to buffer and advance buffer
        fprintf(stdout, "%c%lu", data.characters[i], data.counts[i]); 
    }
}

// Function to compress input string using parallel RLE compression
void parallel_RLE(char* input, size_t length) {
    
    // Calculate number of chunks and the size of allocated memory
    size_t num_chunks = ceil((double)length / MAX_CHUNK_SIZE);
    size_t num_threads = fmin(MAX_NUM_THREADS, num_chunks);
    
    // Initialize a task queue
    task_queue_t queue = create_task_queue(num_threads * 2); // Capacity is twice the number of threads
    
    // Create an empty array of pointers to output
    rle_data_t output[num_chunks]; 
        
    // Create threads
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) { 
        
        // Initialize a thread
        pthread_create(&threads[i], NULL, thread_worker, &queue); 
    }
    
    // Generate tasks
    for (int i = 0; i < num_chunks; i++) {
        
        // Initialize output
        output[i] = create_rle_data(INIT_CAPACITY);
        
        // Create a task
        size_t task_length = fmin(MAX_CHUNK_SIZE, length - i * MAX_CHUNK_SIZE);
        rle_task_t task = create_rle_task(&input[i * MAX_CHUNK_SIZE], task_length, &output[i]); 
        
        // Add the task to the task queue to be processed
        // (this will wait if there is no room in the queue)
        add_task(&queue, task);
    }
    
    // Send termination signal to all threads
    for (int i = 0; i < num_threads; i++) {
        
        // Get a termination task 
        rle_task_t termination_task = get_termination_task(); 
        
        // Add the termination task to the task queue
        add_task(&queue, termination_task); 
        
    }
        
    // Wait until all threads have terminated
    for (int i = 0; i < num_threads; i++) {
        
        // Wait for thread to return
        pthread_join(threads[i], NULL);
    }

    // Combine output
    for (int i = 0; i < num_chunks - 1; i++) {
                
        // Combine subsequent outputs at the boundary
        handle_boundary(&output[i], &output[i + 1]); 
                
        // Write the combined data to standard output
        write_text_to_buffer(stdout, output[i]); 
    }
    
    // Write the last output to standard output
    write_text_to_buffer(stdout, output[num_chunks - 1]); 
}