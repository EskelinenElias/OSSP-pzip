#include "../../include/result_queue/reserve_spot.h"

// Function to reserve a spot in the result queue
size_t reserve_spot(result_queue_t* result_queue) {
    
    // Input validation
    if (!result_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to reserve spot: invalid input\n");
        return INVALID_INDEX;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(result_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to reserve spot: failed to acquire lock\n");
        return INVALID_INDEX;
    }
    
    // Check if there is room in the queue
    while (result_queue->size == result_queue->capacity) {
        
        // Wait until there is room in the queue
        if (pthread_cond_wait(result_queue->room_available, result_queue->lock) != SUCCESS) {
            
            // Failed to wait until there is room in the queue
            fprintf(stderr, "Failed to reserve spot: failed to wait until there is room in the queue\n");
            pthread_mutex_unlock(result_queue->lock); 
            return INVALID_INDEX;
        }
    }
    
    // Reserve the index at the rear of the queue
    size_t reserved_index = result_queue->rear; 
    
    // Check that the reserved spot is actually empty
    if (result_queue->results[reserved_index] != NULL || result_queue->status_flags[reserved_index] != EMPTY) {
        
        // The reserved index is not empty
        fprintf(stderr, "Failed to reserve spot: something went wrong\n"); 
        free_result(result_queue->results[reserved_index]); 
        result_queue->results[reserved_index] = NULL; 
        return INVALID_INDEX; 
    }
    
    // Initialize the result and it's status flag
    result_queue->results[reserved_index] = NULL; 
    result_queue->status_flags[reserved_index] = RESERVED; 
    
    // Increment queue size and update rear index
    result_queue->size++;
    result_queue->rear = (result_queue->rear + 1) % result_queue->capacity;
        
    // Release the lock
    if (pthread_mutex_unlock(result_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to reserve spot: failed to release lock\n");
        result_queue->size--; 
        result_queue->rear = reserved_index;
        pthread_mutex_unlock(result_queue->lock);
        return INVALID_INDEX;
    }
    
    // Successfully reserved a spot in the result queue
    return reserved_index;
}

// EOF