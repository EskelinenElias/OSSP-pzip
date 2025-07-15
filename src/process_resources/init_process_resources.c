#include "../../include/process_resources/init_process_resources.h"

// Function to initialize process variables
process_resources_t* init_process_resources(size_t num_cores, size_t num_files) {
        
    // Input validation
    if (num_cores < 1 || num_files < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize process, invalid input\n"); 
        return NULL;
    }
    
    // Allocate memory for process variables and check for errors
    process_resources_t* process = calloc(1, sizeof(process_resources_t));
    if (!process) {
        
        // Failed to allocate memory for process
        fprintf(stderr, "Failed to initialize process, failed to allocate memory\n");
        return NULL;
    }
    
    // Initialize file manager and check for errors
    if (!(process->file_manager = init_file_manager(num_files))) {
        
        // Failed to initialize file manager
        fprintf(stderr, "Failed to initialize process: failed to initialize file manager\n");
        free_process_resources(process); 
        return NULL;
    }
    
    // Initialize tasks queue and check for errors
    if (!(process->tasks_queue = init_tasks_queue(QUEUE_CAPACITY))) {
        
        // Failed to initialize task task_manager
        fprintf(stderr, "Failed to initialize process: failed to initialize tasks queue\n");
        free_process_resources(process); 
        return NULL;
    }
    
    // Initialize results queue and check for errors
    if (!(process->results_queue = init_results_queue(QUEUE_CAPACITY))) {
        
        // Failed to initialize task task_manager
        fprintf(stderr, "Failed to initialize process: failed to initialize results queue\n");
        free_process_resources(process); 
        return NULL;
    }
    
    // Initialize workers
    size_t num_workers = fmax(fmin(get_num_cores(), MAX_THREADS - 2), 1);
    if (!(process->workers = init_worker_group(num_workers, process->tasks_queue, process->results_queue))) {
        
        // Failed to initialize workers
        fprintf(stderr, "Failed to initialize process: failed to initialize workers\n");
        free_process_resources(process); 
        return NULL;
    }
    
    // Initialize writer and check for errors
    if (!(process->writer = init_writer_thread(process->results_queue, process->file_manager))) {
        
        // Failed to initialize writer
        fprintf(stderr, "Failed to initialize process: failed to initialize writer\n");
        free_process_resources(process); 
        return NULL;
    }
        
    // Successfully initialized process
    return process;
}; 

// EOF