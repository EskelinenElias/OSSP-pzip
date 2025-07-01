#include "../../include/results_queue/yield_result.h"

// Function to yield result to results queue
int yield_result(results_queue_t* results_queue, result_data_t* result_data, size_t reserved_index) {
    
    // Input validation
    if (!results_queue || !results_queue->lock || !results_queue->result_available || reserved_index < 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield result to results queue: invalid input\n");
        return ERROR; 
    } 
    
    // Acquire lock
    if (pthread_mutex_lock(results_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield result to results queue: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check that the reserved index is actually empty
    if (results_queue->results[reserved_index] != NULL || results_queue->status_flags[reserved_index] == COMPLETED) {
        
        // Reserved spot is not empty
        fprintf(stderr, "Failed to yield result to results queue: something went wrong\n"); 
        return ERROR; 
    }
    
    // Add the result to the reserved spot and set it's status as completed
    results_queue->results[reserved_index] = result_data; 
    results_queue->status_flags[reserved_index] = COMPLETED; 
    
    // Check if the result is at the front of the queue
    if (reserved_index == results_queue->front) {
    
        // Signal that the next result is available
        if (pthread_cond_signal(results_queue->result_available) != SUCCESS) {
            
            // Failed to signal that the next result is available
            fprintf(stderr, "Failed to yield result to results queue: failed to signal that the next result is available\n");
            results_queue->results[reserved_index] = NULL; 
            pthread_mutex_unlock(results_queue->lock);
            return ERROR;
        }
    }
    
    // Release the lock
    if (pthread_mutex_unlock(results_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield result to results queue: failed to release lock\n");
        results_queue->results[reserved_index] = NULL; 
        pthread_mutex_unlock(results_queue->lock);
        return ERROR;
    }
    
    // Successfully yielded result
    return SUCCESS;
}