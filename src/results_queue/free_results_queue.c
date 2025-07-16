#include "../../include/result_queue/free_result_queue.h"

// Function to free the result queue
int free_result_queue(result_queue_t* result_queue) {
    
    // Input validation
    if (!result_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free result queue: invalid input\n");
        return ERROR; 
    }
    
    // Check if mutex lock exists
    if (result_queue->lock) {
        
        // Destroy mutex lock for result_queue
        pthread_mutex_destroy(result_queue->lock);
        free(result_queue->lock);
    }
    
    // Check if condition variable for room availability exists
    if (result_queue->room_available) {
        
        // Destroy condition variable for room availability
        pthread_cond_destroy(result_queue->room_available);
        free(result_queue->room_available);
    }   
    
    // Check if condition variable for result_queue availability exists
    if (result_queue->result_available) {
        
        // Destroy condition variable for result_queue availability
        pthread_cond_destroy(result_queue->result_available);
        free(result_queue->result_available);
    }
    
    // Free any remaining result
    for (size_t i = 0; i < result_queue->capacity; i++) {
        
        // If the result exists, free it
        if (result_queue->results[i]) free_result(result_queue->results[i]);
    }
    
    // Free memory allocated for result
    if (result_queue->results) free(result_queue->results);
    
    // Free memory allocated for status flags
    if (result_queue->status_flags) free(result_queue->status_flags);
    
    // Free memory allocated for result queue structure
    free(result_queue); 
    
    // Successfully freed result queue
    return SUCCESS; 
}

// EOF