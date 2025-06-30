#include "../../include/writer/init_writer.h"

// Function to initialize writer thread
pthread_t* init_writer(file_manager_t* file_manager, task_manager_t* task_manager) {
    
    // Allocate memory for writer thread and check for errors
    pthread_t* writer = (pthread_t*)malloc(sizeof(pthread_t));
    if (!writer) {
        
        // Failed to allocate memory for writer thread
        fprintf(stderr, "Failed to initialize writer: failed to allocate memory for writer\n");
        return NULL;
    }
    
    // Allocate memory for writer arguments
    writer_args_t* args = malloc(sizeof(writer_args_t));
    if (!args) {
        
        // Failed to allocate memory for writer arguments
        fprintf(stderr, "Failed to initialize writer: failed to allocate memory for arguments\n");
        free(writer);
        return NULL;
    }
    
    // Set writer arguments
    args->file_manager = file_manager;
    args->task_manager = task_manager;
    
    // Initialize writer thread
    if (pthread_create(writer, NULL, writer_loop, args) != 0) {
        
        // Failed to create writer thread
        fprintf(stderr, "Failed to create writer thread\n");
        free(writer);
        free(args);
        return NULL;
    }
    
    // Successfully initialized writer thread
    return writer;
}

// EOF