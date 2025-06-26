#include "../../include/task_manager/claim_result_data.h"

// Function to claim a result from the task manager (in order)
result_data_t* claim_result_data(task_manager_t* manager) {
        
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim result: invalid input\n");
        return NULL;
    }
        
    // Acquire lock
    if (pthread_mutex_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim result: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if the next result is available
    while (manager->results_queue[manager->results_front] == NULL || manager->results_queue[manager->results_front]->status != COMPLETED) {
                
        // Wait for the next result to become available
        if (pthread_cond_wait(manager->next_result_available, manager->lock) != SUCCESS) {
            
            // Failed to wait for result
            fprintf(stderr, "Failed to claim result: failed to wait for result\n");
            pthread_mutex_unlock(manager->lock);
            return NULL;
        }
    }
    
    // Get result data and free the result
    result_data_t* data = manager->results_queue[manager->results_front]->data;
    manager->results_queue[manager->results_front]->data = NULL; // Set the result data to NULL (to avoid dangling pointers)
    
    // Free the result and set the result array pointer to NULL (to avoid dangling pointers)
    free_result(manager->results_queue[manager->results_front]);
    manager->results_queue[manager->results_front] = NULL; 

    // Increment next result index and decrement current size
    manager->results_front = (manager->results_front + 1) % manager->capacity;
    manager->size--; 
    
    // Signal that the task manager has room available
    if (pthread_cond_signal(manager->room_available) != 0) {
        
        // Failed to signal room available
        fprintf(stderr, "Failed to claim result: failed to signal that room is available\n");
        pthread_mutex_unlock(manager->lock);
        free_result_data(data); 
        return NULL;
    }
    
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim result: failed to release lock\n");
        free_result_data(data); 
        return NULL;
    }
        
    // Return claimed result
    return data;
}