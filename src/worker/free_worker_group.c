#include "../../include/worker/free_worker_group.h"

// Function to free workers
int free_worker_group(worker_group_t* group, tasks_queue_t* tasks_queue) {
    
    // Input validation
    if (!group || !group->workers || group->num_workers <= 0 || !tasks_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free worker group: invalid input\n");
        return ERROR;
    }
    
        
    // Yield NULL tasks to signal termination
    for (size_t i = 0; i < group->num_workers; ++i) {
        
        // Yield NULL task to signal termination
        if (yield_task(tasks_queue, NULL) != SUCCESS) {
            
            // Failed to yield task
            fprintf(stderr, "Failed to free worker group: failed to yield termination task\n");
            return ERROR;
        }
    }
        
    // Wait for workers to finish
    for (size_t i = 0; i < group->num_workers; ++i) {
        
        // Get worker thread
        worker_thread_t* worker = group->workers[i];
        if (!worker) continue; 
        
        // Wait for worker thread to finish
        if (pthread_join(worker->thread, NULL) != SUCCESS) {
            
            // Failed to join worker thread
            fprintf(stderr, "Failed to free worker group: failed to join worker thread\n");
            return ERROR;
        }
        
        // Reset worker thread pointer
        worker->thread = NULL; 
        
        // Free memory allocated for worker thread
        if (free_worker_thread(worker) != SUCCESS) {
            
            // Failed to free worker thread
            fprintf(stderr, "Failed to free worker group: failed to free worker thread\n");
            return ERROR;
        }
    }
    
    // Free memory allocated for worker group
    free(group->workers);
    free(group);

    // Successfully freed workers
    return SUCCESS;
}

// EOF