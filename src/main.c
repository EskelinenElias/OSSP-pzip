#include "../include/main.h"

// Main function 
int main(int argc, char *argv[]) {

    /* 1. VALIDATE INPUT */

    // Check that the correct number of argumen ts were provided
    if (argc < 2) {
        fprintf(stdout, "pzip: file1 [file2 ...]\n");
        return 1;
    }

    /* 2. INITIALIZE THREADS AND TASK QUEUE */

    // Initialize a task queue
    size_t num_workers = get_num_cores(); // Get the number of available CPU cores
    task_queue_t queue = create_task_queue(num_workers * 2); // Capacity is twice the number of threads
    
    // Initialize worker threads
    pthread_t threads[num_workers];
    if (init_workers(threads, num_workers, &queue) != SUCCESS) {
        fprintf(stderr, "pzip: failed to initialize worker threads\n");
        free_task_queue(&queue); // Free the task queue before exiting
        return FAILURE; // Error
    }

    /* 3. PROCESS FILES ONE BY ONE TO CONSERVE MEMORY */

    // Initialize a pointer to encoded data for handling file boundaries
    encoded_data_t* file_boundary_encoded_data = NULL; 

    // Loop trough the input files
    for (int i = 1; i < argc; i++) {

        /* 3.1. MAP FILE INTO MEMORY */

        // Map the file into memory
        if (!argv[i]) continue; // Check that the argument is not NULL
        mapped_file_t mapped_file = map_file(argv[i]);
        if (mapped_file.data == NULL) {
            fprintf(stderr, "pzip: cannot open file %s\n", argv[i]);
            return 1;
        }

        /* 3.2. ENCODE FILE CONTENTS */

        // Determine chunk size and the number of chunks
        size_t chunk_size = fmin(ceil((double)mapped_file.size / num_workers), MAX_CHUNK_SIZE);
        size_t num_chunks = ceil((double)mapped_file.size / chunk_size);
        encoded_data_t* encoding_results[num_chunks]; // Create an array of pointer to encoded data objects
    
        // Populate the task queue with encoding tasks, which are processed by worker threads
        for (int i = 0; i < num_chunks; i++) {
            
            // Create a task
            encoding_results[i] = create_encoded_data(ceil((double)chunk_size / 2)); // Initialize output for the chunk
            size_t task_size = fmin(chunk_size, mapped_file.size - i * chunk_size);
            encoding_task_t task = create_encoding_task(&mapped_file.data[i * chunk_size], task_size, encoding_results[i]); 

            // Add the task to the task queue to be processed (this will wait until there is room in the queue)
            add_task(&queue, task);
        }

        // Wait until all tasks are processed
        while (queue.size > 0) { usleep(1000); } // Sleep for a short time to avoid busy waiting

        // Unmap the file after all tasks have processed
        unmap_file(&mapped_file);

        /* 3.3. WRITE ENCODED DATA TO OUTPUT STREAM */

        // Handle the boundary between the previous file and the current file
        handle_boundary(file_boundary_encoded_data, encoding_results[0]); 

        // Write the last chunk of the previous file to output stream
        write_encoded_data_to_output(stdout, file_boundary_encoded_data); 
        free_encoded_data(file_boundary_encoded_data); // Free the encoded data

        // Iterate over the processed chunks to handle boundaries and write to output stream
        for (int i = 0; i < num_chunks - 1; i++) {
                    
            // Handle the boundary on subsequent encoded chunks
            handle_boundary(encoding_results[i], encoding_results[i + 1]); 
                    
            // Write the first chunk of the subsequent chunks to output stream
            write_encoded_data_to_output(stdout, encoding_results[i]);             
            free_encoded_data(encoding_results[i]); // Free the encoded data    
            encoding_results[i] = NULL; // Set to NULL to avoid dangling pointer
        }

        // Store the last chunk of encoded data to handle the boundary
        file_boundary_encoded_data = encoding_results[num_chunks-1];
        encoding_results[num_chunks-1] = NULL;

        /* 3.4. CLOSE THE CURRENT FILE */

    }

    /* 4. WRITE THE LAST PIECE OF ENCODED DATA */

    // Write the last piece of encoded data to output stream
    write_encoded_data_to_output(stdout, file_boundary_encoded_data); 
    free_encoded_data(file_boundary_encoded_data); // Free the encoded data

    /* 5. TERMINATE THREADS AND CLEAN UP */
    
    // Send termination signal to all threads
    terminate_workers(threads, num_workers, &queue);  

    // Free the task queue
    free_task_queue(&queue);
    
    return 0;
}