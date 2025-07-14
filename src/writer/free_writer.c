#include "../../include/writer/free_writer.h"

// Function to free writer
int free_writer(pthread_t* writer, results_queue_t* results_queue) {
    
    // Input validation 
    if (!writer || !results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free writer; invalid input"); 
        return ERROR; 
    }
        
    // Reserve an index from the results queue
    size_t reserved_index = reserve_spot(results_queue); 
    if (reserved_index == INVALID_INDEX) {
            
        // Failed to reserve index
        fprintf(stderr, "Failed to free writer: failed to reserve index\n"); 
        return ERROR; 
    }
        
    // Yield a NULL result to signal termination
    if (yield_result(results_queue, NULL, reserved_index) != SUCCESS) {
        
        // Failed to yield NULL result to signal termination
        fprintf(stderr, "Failed to free writer; failed to send termination signal\n");
        return ERROR; 
    } 
        
    // Wait for writer to finish
    if (pthread_join(*writer, NULL) != SUCCESS) {
        
        // Failed to wait for termination
        fprintf(stderr, "Failed to free writer: failed to wait for termination\n");
        return ERROR; 
    }
    
    // Free memory allocated for writer
    free(writer);
    
    // Successfully terminated writer
    return SUCCESS;
}

// EOF