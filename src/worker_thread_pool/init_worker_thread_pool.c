#include "../../include/worker_thread_pool/init_worker_thread_pool.h"

// Function to initialize worker thread pool
worker_thread_pool_t* init_worker_thread_pool(size_t num_workers, tasks_queue_t* tasks_queue, results_queue_t* results_queue) {
    
    // Input validation
    if (num_workers <= 0 || !tasks_queue || !results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize worker thread pool: invalid input\n");
        return NULL; 
    }
    
    // Allocate memory for worker thread pool
    worker_thread_pool_t* worker_pool = malloc(sizeof(worker_thread_pool_t));
    if (!worker_pool) {
        
        // Failed to allocate memory for worker thread pool
        fprintf(stderr, "Failed to initialize worker thread pool: failed to allocate memory for worker thread pool\n");
        return NULL;
    }
    
    // Allocate memory for worker threads
    worker_pool->workers = calloc(num_workers, sizeof(worker_thread_t*));
    if (!worker_pool->workers) {
        
        // Failed to allocate memory for worker threads
        fprintf(stderr, "Failed to initialize worker thread pool: failed to allocate memory for worker threads\n");
        free(worker_pool);
        return NULL;
    }
    
    // Set worker thread pool fields
    worker_pool->tasks_queue = tasks_queue;
    worker_pool->num_workers = num_workers;
    
    // Initialize worker threads
    for (size_t i = 0; i < num_workers; i++) {
        
        // Initialize worker thread
        worker_pool->workers[i] = init_worker_thread(tasks_queue, results_queue);
        if (!worker_pool->workers[i]) {
            
            // Failed to initialize worker thread
            fprintf(stderr, "Failed to initialize worker thread pool: failed to initialize worker %zu\n", i);
            free_worker_thread_pool(worker_pool);
            return NULL;
        }
    }
    
    // Successfully initialized worker thread pool
    return worker_pool;
}

// EOF