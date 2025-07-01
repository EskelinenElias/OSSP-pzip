#include "../../include/writer/terminate_writer.h"
#include <pthread.h>

// Function to terminate the writer thread
int terminate_writer(pthread_t* writer, results_queue_t* results_queue) {
    
    // Input validation 
    if (!writer) {
        
        // Invalid input
        fprintf(stderr, "Failed to terminate writer; invalid input"); 
        return ERROR; 
    }
    
    // Try sending a termination signal to the writer thread
    if (!results_queue) {
        
        // Results queue does not exists; force termination
        if (pthread_cancel(*writer) != SUCCESS) {
            
            // Failed to force termination
            fprintf(stderr, "Failed to terminate writer; failed to force termination\n"); 
            return ERROR; 
        }
    
    } else {
        
        // Reserve an index from the results queue
        size_t reserved_index = reserve_spot(results_queue); 
        if (reserved_index <= 0) {
            
            // Failed to reserve index
            fprintf(stderr, "Failed to terminate writer; failed to reserve index\n"); 
            return ERROR; 
        }
        
        // Yield a NULL result to signal termination
        if (yield_result(results_queue, NULL, reserved_index) != SUCCESS) {
            
            // Failed to yield NULL result to signal termination
            fprintf(stderr, "Failed to terminate writer; failed to send termination signal\n"); 
            return ERROR; 
        } 
        
        // Wait for writer to terminate
        if (pthread_join(*writer, NULL) != SUCCESS) {
            
            // Failed to wait for termination
            fprintf(stderr, "Failed to terminate writer: failed to wait for termination\n"); 
        }
    }
    
    // Free memory allocated for writer
    free(writer);
    
    // Successfully terminated writer
    return SUCCESS;
}

// EOF