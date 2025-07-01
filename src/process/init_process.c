#include "../../include/process/init_process.h"

// Function to initialize process variables
process_vars_t* init_process(size_t num_cores, size_t num_files) {
        
    // Input validation
    if (num_cores < 1 || num_files < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize process, invalid input\n"); 
        return NULL;
    }
    
    // Allocate memory for process variables and check for errors
    process_vars_t* process = (process_vars_t*)malloc(sizeof(process_vars_t));
    if (!process) {
        
        // Failed to allocate memory for process
        fprintf(stderr, "Failed to initialize process, failed to allocate memory\n");
        return NULL;
    }
    
    // Initialize process fields
    process->file_manager = NULL; 
    process->tasks_queue = NULL;
    process->results_queue = NULL; 
    process->workers = NULL; 
    process->num_workers = num_cores - 1 ; 
    process->writer = NULL;
    
    // Initialize file manager and check for errors
    if (!(process->file_manager = init_file_manager(num_files))) {
        
        // Failed to initialize file manager
        fprintf(stderr, "Failed to initialize process: failed to initialize file manager\n");
        free_process(process); 
        return NULL;
    }
    
    // Initialize tasks queue and check for errors
    if (!(process->tasks_queue = init_tasks_queue(MAX_CAPACITY))) {
        
        // Failed to initialize task task_manager
        fprintf(stderr, "Failed to initialize process: failed to initialize tasks queue\n");
        free_process(process); 
        return NULL;
    }
    
    // Initialize results queue and check for errors
    if (!(process->results_queue = init_results_queue(MAX_CAPACITY))) {
        
        // Failed to initialize task task_manager
        fprintf(stderr, "Failed to initialize process: failed to initialize results queue\n");
        free_process(process); 
        return NULL;
    }
    
    // Allocate memory for workers and check for errors
    if (!(process->workers = (pthread_t**)malloc(sizeof(pthread_t*) * process->num_workers))) {
        
        // Failed to allocate memory for workers
        fprintf(stderr, "Failed to initialize process: failed to allocate memory for workers\n");
        free_process(process); 
        return NULL;
    }
    
    // Initialize workers and check for errors
    for (size_t i = 0; i < process->num_workers; i++) {
        
        // Initialize a worker and check for errors
        if (!(process->workers[i] = init_worker(process->tasks_queue, process->results_queue))) {
            
            // Failed to initialize worker
            fprintf(stderr, "Failed to initialize process: failed to initialize worker %zu\n", i);
            free_process(process); 
            return NULL;
        }
    }
    
    // Initialize writer and check for errors
    if (!(process->writer = init_writer(process->file_manager, process->results_queue))) {
        
        // Failed to initialize writer
        fprintf(stderr, "Failed to initialize process: failed to initialize writer\n");
        free_process(process); 
        return NULL;
    }
        
    // Successfully initialized process
    return process;
}; 

// EOF