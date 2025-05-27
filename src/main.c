#include "../include/main.h"

// Main function 
int main(int argc, char *argv[]) {

    /* 1. VALIDATE INPUT */

    // Check that the correct number of argumen ts were provided
    if (argc < 2) {
        fprintf(stdout, "pzip: file1 [file2 ...]\n");
        return 1;
    }

    /* 2. READ FILES TO BUFFER */

    // Initialize a buffer for reading the input files
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t total_read = 0; 
    char* buffer = malloc(buffer_size);
    if (!buffer) {
        fprintf(stderr, "pzip: memory allocation error\n");
        return 1;
    }
    char* current_position = buffer;
    
    // Read the input files to the buffer
    for (int i = 1; i < argc; i++) {

        // Check if the argument is not NULL
        if (!argv[i]) continue;

        // Open input file
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            fprintf(stderr, "pzip: cannot open file\n");
            return 1;
        }

        // Read the file into the buffer and reallocate memory if necessary
        while (!feof(file)) {

            // Increase the buffer size if necessary
            if (current_position >= buffer + buffer_size) {
                buffer_size *= 2;
                char* new_buffer = realloc(buffer, buffer_size);
                if (!new_buffer) {
                    fprintf(stderr, "pzip: memory allocation failed\n");
                    free(buffer);
                    fclose(file);
                    return 1;
                }
                current_position = new_buffer + total_read;
                buffer = new_buffer;
            }
            
            // Read data from the file into the buffer
            size_t chars_read = read_to_buffer(file, current_position, buffer + buffer_size);
            if (chars_read == (size_t)-1) {
                fprintf(stderr, "pzip: error reading file\n");
                fclose(file);
                free(buffer);
                return 1;
            }
            current_position += chars_read;
            total_read += chars_read;
        }

        // Close the input file
        fclose(file);
    }

    /* 3. COMPRESS FILE CONTENTS */

    // Calculate the number of chunks based on the buffer size and maximum chunk size
    size_t num_chunks = ceil((double)buffer_size / MAX_CHUNK_SIZE);
    size_t num_threads = fmin(get_num_cores(), num_chunks);

    printf("pzip: using %d threads\n", get_num_cores());
    
    // Initialize a task queue
    task_queue_t queue = create_task_queue(num_threads * 2); // Capacity is twice the number of threads
    
    // Create an empty array of pointers to output
    encoded_data_t compressed_data[num_chunks]; 
        
    // Create threads
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) { 
        
        // Initialize a thread
        pthread_create(&threads[i], NULL, thread_worker, &queue); 
    }
    
    // Generate tasks
    for (int i = 0; i < num_chunks; i++) {
        
        // Initialize output
        compressed_data[i] = create_encoded_data(INIT_CAPACITY);
        
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