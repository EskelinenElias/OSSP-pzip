#include "../../include/writer/terminate_writer.h"
#include <pthread.h>

// Function to terminate the writer thread
void* terminate_writer(pthread_t* writer, task_manager_t* task_manager) {
    
    // Input validation 
    if (!writer) {
        
        // Invalid input
        fprintf(stderr, "Failed to terminate writer; invalid input"); 
        return NULL; 
    }
    
    // Try sending a termination signal to the writer thread
    if (!task_manager || (send_termination_signal(task_manager) != SUCCESS)) {
        
        // Failed to send termination signal, force termination
        if (pthread_cancel(*writer) != SUCCESS) {
            
            // Failed to force termination
            fprintf(stderr, "Failed to terminate writer; failed to force termination\n"); 
        }
        return NULL; 
    }
    
    // Wait for writer to terminate
    if (pthread_join(*writer, NULL) != SUCCESS) {
        
        // Failed to wait for termination
        fprintf(stderr, "Failed to terminate writer: failed to wait for termination\n"); 
    }
    return NULL;
}

// EOF