#include "../../include/worker/free_workers.h"

// Function to free workers
int free_workers(pthread_t** workers, size_t num_workers, tasks_queue_t* tasks_queue) {
    
    // Input validation
    if (!workers || num_workers <= 0 || !tasks_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free workers: invalid input\n");
        return ERROR;
    }
    
        
    // Yield NULL tasks to signal termination
    for (size_t i = 0; i < num_workers; ++i) {
        
        // Yield NULL task to signal termination
        if (yield_task(tasks_queue, NULL) != SUCCESS) {
            
            // Failed to yield task
            fprintf(stderr, "Failed to free workers: failed to yield termination task\n");
            return ERROR;
        }
    }
        
    // Wait for workers to finish
    for (size_t i = 0; i < num_workers; ++i) {
        
        // Wait for worker to finish
        if (pthread_join(*workers[i], NULL) != SUCCESS) {
            
            // Failed to join worker
            fprintf(stderr, "Failed to free workers: failed to join worker\n");
            return ERROR;
        }
    }
    
    // Free memory allocated for workers
    for (size_t i = 0; i < num_workers; ++i) {
        
        // Free memory allocated for worker
        free(workers[i]);
        workers[i] = NULL;
    }

    // Successfully freed workers
    return SUCCESS;
}

// EOF