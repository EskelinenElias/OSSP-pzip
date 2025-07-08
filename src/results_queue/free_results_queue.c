#include "../../include/results_queue/free_results_queue.h"

// Function to free the results queue
int free_results_queue(results_queue_t* results_queue) {
    
    // Input validation
    if (!results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free results queue: invalid input\n");
        return ERROR; 
    }
    
    // Check if mutex lock exists
    if (results_queue->lock) {
        
        // Destroy mutex lock for results_queue
        pthread_mutex_destroy(results_queue->lock);
        free(results_queue->lock);
    }
    
    // Check if condition variable for room availability exists
    if (results_queue->room_available) {
        
        // Destroy condition variable for room availability
        pthread_cond_destroy(results_queue->room_available);
        free(results_queue->room_available);
    }   
    
    // Check if condition variable for results_queue availability exists
    if (results_queue->result_available) {
        
        // Destroy condition variable for results_queue availability
        pthread_cond_destroy(results_queue->result_available);
        free(results_queue->result_available);
    }
    
    // Free any remaining results
    for (size_t i = 0; i < results_queue->capacity; i++) {
        
        // If the result exists, free it
        if (results_queue->results[i]) free_result(results_queue->results[i]);
    }
    
    // Free memory allocated for results
    if (results_queue->results) free(results_queue->results);
    
    // Free memory allocated for status flags
    if (results_queue->status_flags) free(results_queue->status_flags);
    
    // Free memory allocated for results queue structure
    free(results_queue); 
    
    // Successfully freed results queue
    return SUCCESS; 
}

// EOF