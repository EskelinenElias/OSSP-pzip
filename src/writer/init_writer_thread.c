#include "../../include/writer/init_writer_thread.h"

// Function to initialize writer thread
writer_thread_t* init_writer_thread(results_queue_t* results_queue, file_manager_t* file_manager) {
    
    // Input validation
    if (!results_queue || !file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize writer thread: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for writer thread
    writer_thread_t* writer = malloc(sizeof(writer_thread_t));
    if (!writer) {
        
        // Failed to allocate memory for writer thread
        fprintf(stderr, "Failed to initialize writer thread: failed to allocate memory for writer thread\n");
        return NULL;
    }
    
    // Set writer arguments
    writer->file_manager = file_manager;
    writer->results_queue = results_queue;
    writer->current_result = NULL; 
    writer->next_result = NULL; 
    
    // Initialize writer thread
    if (pthread_create(&writer->thread, NULL, process_results, writer) != 0) {
        
        // Failed to create writer thread
        fprintf(stderr, "Failed to create writer thread\n");
        free(writer);
        return NULL;
    }
    
    // Successfully initialized writer thread
    return writer;
}

// EOF