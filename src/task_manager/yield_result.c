#include "../../include/task_manager/yield_result.h"

// Function to yield the result of a task to the manager (by a worker)
int yield_result(task_manager_t* manager, result_t* result, result_data_t* result_data) {
    
    // Input validation
    if (!manager || !result) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield result: invalid input\n");
        return ERROR; 
    } 
    
    // Acquire lock
    if (pthread_mutex_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield result: failed to acquire lock\n");
        return ERROR;
    }
    
    // Add the result to the managers results_queue array
    result->data = result_data;
    result->status = COMPLETED; // Set the status flag to COMPLETED
    
    // Check if the result is the next result in the queue
    if (manager->results_queue[manager->results_front] == result) {
    
        // Signal that the next result is available
        if (pthread_cond_signal(manager->next_result_available) != SUCCESS) {
            
            // Failed to signal that the next result is available
            fprintf(stderr, "Failed to yield result: failed to signal that the next result is available\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR;
        }
    }
    
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield result: failed to release lock\n");
        return ERROR;
    }
    
    // Successfully yielded result
    return SUCCESS;
}