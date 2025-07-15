#include "../../include/writer/init_writer_thread.h"

// Function to initialize writer thread
writer_thread_t* init_writer_thread(file_manager_t* file_manager, results_queue_t* results_queue) {
    
    // Input validation
    if (!file_manager || !results_queue) {
        
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
    
    // Allocate memory for writer thread resources
    writer->resources = init_writer_thread_resources();
    if (!writer->resources) {
        
        // Failed to allocate memory for writer thread resources
        fprintf(stderr, "Failed to initialize writer thread: failed to allocate memory for writer thread resources\n");
        free(writer);
        return NULL;
    }

    // Allocate memory for writer thread arguments
    writer_thread_args_t* writer_args = malloc(sizeof(writer_thread_args_t));
    if (!writer_args) {
        
        // Failed to allocate memory for writer thread arguments
        fprintf(stderr, "Failed to initialize writer thread: failed to allocate memory for writer thread arguments\n");
        free(writer->resources);
        free(writer);
        return NULL;
    }
    
    // Set writer arguments
    writer_args->file_manager = file_manager;
    writer_args->results_queue = results_queue;
    writer_args->writer = writer;
    
    // Initialize writer thread
    if (pthread_create(&writer->thread, NULL, process_results, writer_args) != 0) {
        
        // Failed to create writer thread
        fprintf(stderr, "Failed to create writer thread\n");
        free(writer->resources);
        free(writer);
        free(writer_args);
        return NULL;
    }
    
    // Successfully initialized writer thread
    return writer;
}

// EOF