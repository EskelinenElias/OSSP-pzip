#include "../../include/writer_thread/free_writer_thread.h"

// Function to free writer thread
int free_writer_thread(writer_thread_t* writer) {
    
    // Input validation 
    if (!writer || !writer->thread || !writer->result_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free writer thread; invalid input"); 
        return ERROR; 
    }
        
    // Yield a NULL result to signal termination
    if (yield_NULL_result(writer->result_queue) != SUCCESS) {
        
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
    
    // Free memory allocated for writer thread resources and writer thread
    if (writer->current_result) free_encoding_result(writer->current_result); 
    if (writer->next_result) free_encoding_result(writer->next_result); 
    free(writer);
    
    // Successfully terminated writer
    return SUCCESS;
}

// EOF