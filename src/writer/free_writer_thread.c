#include "../../include/writer/free_writer_thread.h"

// Function to free writer thread
int free_writer_thread(writer_thread_t* writer, results_queue_t* results_queue) {
    
    // Input validation 
    if (!writer || !writer->thread || !writer->resources || !results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free writer thread; invalid input"); 
        return ERROR; 
    }
        
    // Reserve an index from the results queue
    size_t reserved_index = reserve_spot(results_queue); 
    if (reserved_index == INVALID_INDEX) {
            
        // Failed to reserve index
        fprintf(stderr, "Failed to free writer thread: failed to reserve index\n"); 
        return ERROR; 
    }
        
    // Yield a NULL result to signal termination
    if (yield_result(results_queue, NULL, reserved_index) != SUCCESS) {
        
        // Failed to yield NULL result to signal termination
        fprintf(stderr, "Failed to free writer thread; failed to send termination signal\n");
        return ERROR; 
    } 
        
    // Wait for writer thread to finish
    if (pthread_join(writer->thread, NULL) != SUCCESS) {
        
        // Failed to wait for termination
        fprintf(stderr, "Failed to free writer thread: failed to wait for termination\n");
        return ERROR; 
    }
    
    // Free memory allocated for writer resources and writer
    free_writer_thread_resources(writer->resources); 
    free(writer);
    
    // Successfully terminated writer
    return SUCCESS;
}

// EOF