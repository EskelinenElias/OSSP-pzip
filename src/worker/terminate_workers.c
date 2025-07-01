#include "../../include/worker/terminate_workers.h"

// Function to terminate workers
int terminate_workers(pthread_t** workers, size_t num_workers, tasks_queue_t* tasks_queue) {
    
    // Input validation
    if (!workers || num_workers <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to terminate workers: invalid input\n");
        return ERROR;
    }
    
    // Check if the task queue still exists
    if (!tasks_queue) {
        
        // Task queue does not exist; force termination
        for (size_t i = 0; i < num_workers; ++i) pthread_cancel(*workers[i]);

    } else {
        
        // Task queue exists; yield NULL tasks to signal termination
        for (size_t i = 0; i < num_workers; ++i) {
            
            // Yield NULL task to signal termination
            if (yield_task(tasks_queue, NULL) != SUCCESS) {
                
                // Failed to yield task
                fprintf(stderr, "Failed to terminate workers: failed to yield task\n");
                return ERROR;
            };
        }
        
        // Wait for workers to finish
        for (size_t i = 0; i < num_workers; ++i) {
            
            // Wait for worker to finish
            pthread_join(*workers[i], NULL);
        }
    }
    
    // Free memory allocated for workers
    for (size_t i = 0; i < num_workers; ++i) free(workers[i]);

    // Successfully terminated workers
    return SUCCESS;
}

// EOF