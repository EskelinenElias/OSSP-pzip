#include "../../include/worker_thread_pool/free_worker_thread_pool.h"

// Function to free workers
int free_worker_thread_pool(worker_thread_pool_t* worker_pool) {
    
    // Input validation
    if (!worker_pool || !worker_pool->workers || worker_pool->num_workers <= 0 || !worker_pool->task_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free worker pool: invalid input\n");
        return ERROR;
    }
    
        
    // Yield NULL task to signal termination
    for (size_t i = 0; i < worker_pool->num_workers; ++i) {
        
        // Yield NULL task to signal termination
        if (yield_NULL_task(worker_pool->task_queue) != SUCCESS) {
            
            // Failed to yield task
            fprintf(stderr, "Failed to free worker pool: failed to yield termination task\n");
            return ERROR;
        }
    }
        
    // Wait for workers to finish
    for (size_t i = 0; i < worker_pool->num_workers; ++i) {
        
        // Get worker thread
        worker_thread_t* worker = worker_pool->workers[i];
        if (!worker) continue; 
        
        // Wait for worker thread to finish
        if (pthread_join(worker->thread, NULL) != SUCCESS) {
            
            // Failed to join worker thread
            fprintf(stderr, "Failed to free worker pool: failed to join worker thread\n");
            return ERROR;
        }
        
        // Reset worker thread pointer
        worker->thread = NULL; 
        
        // Free memory allocated for worker thread
        if (free_worker_thread(worker) != SUCCESS) {
            
            // Failed to free worker thread
            fprintf(stderr, "Failed to free workerpool: failed to free worker thread\n");
            return ERROR;
        }
    }
    
    // Free memory allocated for worker pool
    free(worker_pool->workers);
    free(worker_pool);

    // Successfully freed workers
    return SUCCESS;
}

// EOF