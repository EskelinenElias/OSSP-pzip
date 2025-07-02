#include "../../include/results_queue/reserve_spot.h"

// Function to reserve a spot in the results queue
size_t reserve_spot(results_queue_t* results_queue) {
    
    // Input validation
    if (!results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to reserve spot: invalid input\n");
        return -1;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(results_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to reserve spot: failed to acquire lock\n");
        return -1;
    }
    
    // Check if there is room in the queue
    while (results_queue->size == results_queue->capacity) {
        
        // Wait until there is room in the queue
        if (pthread_cond_wait(results_queue->room_available, results_queue->lock) != SUCCESS) {
            
            // Failed to wait until there is room in the queue
            fprintf(stderr, "Failed to reserve spot: failed to wait until there is room in the queue\n");
            pthread_mutex_unlock(results_queue->lock); 
            return -1;
        }
    }
    
    // Reserve the index at the rear of the queue
    size_t reserved_index = results_queue->rear; 
    
    // Check that the reserved spot is actually empty
    if (results_queue->results[reserved_index]) {
        
        // The reserved index is not empty
        fprintf(stderr, "Failed to reserve spot: something went wrong\n"); 
        free_result(results_queue->results[reserved_index]); 
        results_queue->results[reserved_index] = NULL; 
        return -1; 
    }
    
    // Initialize the result and it's status flag
    results_queue->results[reserved_index] = NULL; 
    results_queue->status_flags[reserved_index] = PENDING; 
    
    // Increment queue size and update rear index
    results_queue->size++;
    results_queue->rear = (results_queue->rear + 1) % results_queue->capacity;
        
    // Release the lock
    if (pthread_mutex_unlock(results_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to reserve spot: failed to release lock\n");
        results_queue->size--; 
        results_queue->rear = reserved_index;
        pthread_mutex_unlock(results_queue->lock);
        return -1;
    }
    
    // Successfully reserved a spot in the results queue
    return reserved_index;
}

// EOF