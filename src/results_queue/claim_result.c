#include "../../include/results_queue/claim_result.h"

// Function to claim result from results queue
result_data_t* claim_result(results_queue_t* results_queue) {
        
    // Input validation
    if (!results_queue || !results_queue->lock || !results_queue->results || !results_queue->room_available) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim result from results queue: invalid input\n");
        return NULL;
    }
        
    // Acquire lock
    if (pthread_mutex_lock(results_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim result from results queue: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if the next result is available
    while (results_queue->status_flags[results_queue->front] != COMPLETED) {
                
        // Wait for the next result to become available
        if (pthread_cond_wait(results_queue->result_available, results_queue->lock) != 0) {
            
            // Failed to wait for result
            fprintf(stderr, "Failed to claim result from results queue: failed to wait for result\n");
            pthread_mutex_unlock(results_queue->lock);
            return NULL;
        }
    }
    
    // Get result from the queue and set the slot in the queue to NULL
    result_data_t* result_data = results_queue->results[results_queue->front];
    results_queue->results[results_queue->front] = NULL;
    
    // Decrement results queue size and update results queue front indedx
    results_queue->front = (results_queue->front + 1) % results_queue->capacity;
    results_queue->size--; 
    
    // Signal that there is room available in the queue
    if (pthread_cond_signal(results_queue->room_available) != 0) {
        
        // Failed to signal room available
        fprintf(stderr, "Failed to claim result from results queue: failed to signal that room is available\n");
        free_result_data(result_data); 
        pthread_mutex_unlock(results_queue->lock);
        return NULL;
    }
    
    // Release the lock
    if (pthread_mutex_unlock(results_queue->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim result from results queue: failed to release lock\n");
        free_result_data(result_data); 
        pthread_mutex_unlock(results_queue->lock); 
        return NULL;
    }
        
    // Successfully claimed result 
    return result_data;
}

// EOF