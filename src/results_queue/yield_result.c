#include "../../include/result_queue/yield_result.h"

// Function to yield result to result queue
int yield_result(result_queue_t* result_queue, result_t* result, size_t reserved_index) {
        
    // Input validation
    if (!result_queue || !result_queue->lock || !result_queue->result_available || reserved_index < 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield result to result queue: invalid input\n");
        return ERROR; 
    } 
    
    // Acquire lock
    if (pthread_mutex_lock(result_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield result to result queue: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check that the reserved index is actually empty
    if (result_queue->results[reserved_index] != NULL || result_queue->status_flags[reserved_index] != RESERVED) {
        
        // Reserved spot is not empty
        fprintf(stderr, "Failed to yield result to result queue: something went wrong\n"); 
        return ERROR; 
    }
    
    // Add the result to the reserved spot and set it's status as completed
    result_queue->results[reserved_index] = result; 
    result_queue->status_flags[reserved_index] = COMPLETED; 
    
    // Signal that the next result is available
    if (reserved_index == result_queue->front && pthread_cond_signal(result_queue->result_available) != 0) {
        
        // Failed to signal that the next result is available
        fprintf(stderr, "Failed to yield result to result queue: failed to signal that the next result is available\n");
        result_queue->results[reserved_index] = NULL; 
        pthread_mutex_unlock(result_queue->lock);
        return ERROR;
    }
    
    // Release the lock
    if (pthread_mutex_unlock(result_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield result to result queue: failed to release lock\n");
        result_queue->results[reserved_index] = NULL; 
        pthread_mutex_unlock(result_queue->lock);
        return ERROR;
    }
    
    // Successfully yielded result
    return SUCCESS;
}