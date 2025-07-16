#include "../../include/main_thread_resources/init_main_thread_resources.h"

// Function to initialize main thread resources
main_thread_resources_t* init_main_thread_resources(size_t num_cores, size_t num_files) {
        
    // Input validation
    if (num_cores < 1 || num_files < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize main thread resources: invalid input\n"); 
        return NULL;
    }
    
    // Allocate memory for main thread resources
    main_thread_resources_t* resources = calloc(1, sizeof(main_thread_resources_t));
    if (!resources) {
        
        // Failed to allocate memory for main thread resources
        fprintf(stderr, "Failed to initialize main thread resources: failed to allocate memory for main thread resources\n");
        return NULL;
    }
    
    // Initialize file manager
    if (!(resources->file_manager = init_file_manager(num_files))) {
        
        // Failed to initialize file manager
        fprintf(stderr, "Failed to initialize main thread resources: failed to initialize file manager\n");
        free_main_thread_resources(resources); 
        return NULL;
    }
    
    // Initialize tasks queue 
    if (!(resources->tasks_queue = init_tasks_queue(QUEUE_CAPACITY))) {
        
        // Failed to initialize tasks queue
        fprintf(stderr, "Failed to initialize main thread resources: failed to initialize tasks queue\n");
        free_main_thread_resources(resources); 
        return NULL;
    }
    
    // Initialize results queue
    if (!(resources->results_queue = init_results_queue(QUEUE_CAPACITY))) {
        
        // Failed to initialize results queue
        fprintf(stderr, "Failed to initialize main thread resources: failed to initialize results queue\n");
        free_main_thread_resources(resources); 
        return NULL;
    }
    
    // Initialize worker thread pool
    size_t num_workers = fmax(fmin(get_num_cores(), MAX_THREADS - 2), 1);
    if (!(resources->worker_pool = init_worker_thread_pool(num_workers, resources->tasks_queue, resources->results_queue))) {
        
        // Failed to initialize worker thread pool
        fprintf(stderr, "Failed to initialize main thread resources: failed to initialize worker thread pool\n");
        free_main_thread_resources(resources); 
        return NULL;
    }
    
    // Initialize writer thread
    if (!(resources->writer = init_writer_thread(resources->results_queue, resources->file_manager))) {
        
        // Failed to initialize writer thread
        fprintf(stderr, "Failed to initialize main thread resources: failed to initialize writer thread\n");
        free_main_thread_resources(resources); 
        return NULL;
    }
    
    // Initialize task
    resources->task = NULL; 
        
    // Successfully initialized main thread resources
    return resources;
}; 

// EOF