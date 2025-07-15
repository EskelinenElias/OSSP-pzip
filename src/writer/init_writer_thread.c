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
    writer_thread_t* writer_thread = malloc(sizeof(writer_thread_t));
    if (!writer_thread) {
        
        // Failed to allocate memory for writer thread
        fprintf(stderr, "Failed to initialize writer thread: failed to allocate memory for writer thread\n");
        return NULL;
    }
    
    // Allocate memory for writer thread resources
    writer_thread->resources = init_writer_thread_resources();
    if (!writer_thread->resources) {
        
        // Failed to allocate memory for writer thread resources
        fprintf(stderr, "Failed to initialize writer thread: failed to allocate memory for writer thread resources\n");
        free(writer_thread);
        return NULL;
    }

    // Allocate memory for writer thread arguments
    writer_args_t* writer_thread_args = malloc(sizeof(writer_args_t));
    if (!writer_thread_args) {
        
        // Failed to allocate memory for writer thread arguments
        fprintf(stderr, "Failed to initialize writer thread: failed to allocate memory for writer thread arguments\n");
        free(writer_thread->resources);
        free(writer_thread);
        return NULL;
    }
    
    // Set writer arguments
    writer_thread_args->file_manager = file_manager;
    writer_thread_args->results_queue = results_queue;
    writer_thread_args->resources = writer_thread->resources;
    
    // Initialize writer thread
    if (pthread_create(&writer_thread->thread, NULL, process_results, writer_thread_args) != 0) {
        
        // Failed to create writer thread
        fprintf(stderr, "Failed to create writer thread\n");
        free(writer_thread->resources);
        free(writer_thread);
        free(writer_thread_args);
        return NULL;
    }
    
    // Successfully initialized writer thread
    return writer_thread;
}

// EOF