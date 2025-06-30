#include "../../include/process/free_process.h"

// Function to free process variables
void* free_process(process_vars_t* process) {
        
    // Send termination signals
    for (size_t i = 0; i < process->num_workers; i++) {
        send_termination_signal(process->task_manager);
    }
        
    // Wait for all threads to complete
    if (process->workers) {
        for (size_t i = 0; i < process->num_workers; i++) {
            
            // Check if the worker still exists
            if (process->workers[i]) {
                
                // Wait for worker to complete
                pthread_join(*process->workers[i], NULL);
                
                // Set the worker to NULL
                process->workers[i] = NULL;
            }
        }
    }
    
    // Terminate writer thread
    if (process->writer) terminate_writer(process->writer, process->task_manager);
    
    // Free task manager
    if (process->task_manager) free_task_manager(process->task_manager);
    
    // Free file manager
    if (process->file_manager) free_file_manager(process->file_manager);
            
    // Free process variables
    free(process);
    return NULL; 
}

// EOF