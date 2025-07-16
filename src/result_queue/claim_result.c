#include "../../include/result_queue/claim_result.h"

// Function to claim result from result queue
encoding_result_t* claim_result(result_queue_t* result_queue) {
            
    // Input validation
    if (!result_queue || !result_queue->lock || !result_queue->results || !result_queue->room_available) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim result from result queue: invalid input\n");
        return NULL;
    }
        
    // Acquire lock
    if (pthread_mutex_lock(result_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim result from result queue: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if the next result is available
    while (result_queue->status_flags[result_queue->front] != COMPLETED) {
                
        // Wait for the next result to become available
        if (pthread_cond_wait(result_queue->result_available, result_queue->lock) != 0) {
            
            // Failed to wait for result
            fprintf(stderr, "Failed to claim result from result queue: failed to wait for result\n");
            pthread_mutex_unlock(result_queue->lock);
            return NULL;
        }
    }
    
    // Get result from the queue and set the slot in the queue to NULL
    encoding_result_t* result = result_queue->results[result_queue->front];
    result_queue->results[result_queue->front] = NULL;
    result_queue->status_flags[result_queue->front] = EMPTY;
    
    // Decrement result queue size and update result queue front indedx
    result_queue->front = (result_queue->front + 1) % result_queue->capacity;
    result_queue->size--; 
    
    // Signal that there is room available in the queue
    if (pthread_cond_signal(result_queue->room_available) != 0) {
        
        // Failed to signal room available
        fprintf(stderr, "Failed to claim result from result queue: failed to signal that room is available\n");
        free_encoding_result(result); 
        pthread_mutex_unlock(result_queue->lock);
        return NULL;
    }
    
    // Release the lock
    if (pthread_mutex_unlock(result_queue->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim result from result queue: failed to release lock\n");
        free_encoding_result(result); 
        pthread_mutex_unlock(result_queue->lock); 
        return NULL;
    }
        
    // Successfully claimed result 
    return result;
}

// EOF