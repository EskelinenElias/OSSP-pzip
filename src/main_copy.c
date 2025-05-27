#include "../include/main_copy.h"

// Main function 
int main(int argc, char *argv[]) {

    /* 1. VALIDATE INPUT */

    // Check that the correct number of argumen ts were provided
    if (argc < 2) {
        fprintf(stdout, "pzip: file1 [file2 ...]\n");
        return 1;
    }

    /* 2. PROCESS FILES ONE BY ONE TO CONSERVE MEMORY */

    // Loop trough the input files
    for (int i = 1; i < argc; i++) {

        /* 2.1. MAP FILE INTO MEMORY */

        // Check that the argument is not NULL
        if (!argv[i]) continue;

        // Map the file into memory
        mapped_file_t mapped_file = map_file(argv[i]);
        if (mapped_file.data == NULL) {
            fprintf(stderr, "pzip: cannot open file %s\n", argv[i]);
            return 1;
        }

        /* 2.2. ENCODE FILE CONTENTS */

        // Calculate the number of chunks based on the buffer size and maximum chunk size
        size_t num_chunks = ceil((double)mapped_file.size / MAX_CHUNK_SIZE);
        size_t num_threads = fmin(get_num_cores(), num_chunks);
    
        // Initialize a task queue
        task_queue_t queue = create_task_queue(num_threads * 2); // Capacity is twice the number of threads
    
        // Create an empty array of pointers to output
        encoded_data_t encoded_data[num_chunks]; 
        
        // Create threads
        pthread_t threads[num_threads];
        for (int i = 0; i < num_threads; i++) { 
        
            // Initialize a thread
            pthread_create(&threads[i], NULL, thread_worker, &queue); 
        }
    
        // Generate tasks
        for (int i = 0; i < num_chunks; i++) {
            
            // Initialize output
            encoded_data[i] = create_encoded_data(INIT_CAPACITY);
            
            // Create a task
            size_t task_buffer_size = fmin(MAX_CHUNK_SIZE, buffer_size - i * MAX_CHUNK_SIZE);
            encoding_task_t task = create_encoding_task(&buffer[i * MAX_CHUNK_SIZE], task_buffer_size, &compressed_data[i]); 
            
            // Add the task to the task queue to be processed
            // (this will wait if there is no room in the queue)
            add_task(&queue, task);
        }
    
    // Send termination signal to all threads
    for (int i = 0; i < num_threads; i++) {
        
        // Get a termination task 
        encoding_task_t termination_task = get_termination_task(); 
        
        // Add the termination task to the task queue
        add_task(&queue, termination_task); 
        
    }
        
    // Wait until all threads have terminated
    for (int i = 0; i < num_threads; i++) {
        
        // Wait for thread to return
        pthread_join(threads[i], NULL);
    }

    /* 4. COMBINE CHUNKS AND WRITE ENCODED DATA TO OUTPUT STREAM */
    
    // Iterate over the chunks to handle boundaries and write to output stream
    for (int i = 0; i < num_chunks - 1; i++) {
                
        // Handle the boundary on subsequent encoded chunks
        handle_boundary(&compressed_data[i], &compressed_data[i + 1]); 
                
        // Write the first chunk of the subsequent chunks to output stream
        write_text_to_buffer(stdout, compressed_data[i]); 
    }

    // Write the last chunk to output stream
    write_text_to_buffer(stdout, compressed_data[num_chunks - 1]); 

    /* 5. FREE ALLOCATED MEMORY */

    // Free the buffer
    free(buffer);
    return 0;
}