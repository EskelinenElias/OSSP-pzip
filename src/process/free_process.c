#include "../../include/process/free_process.h"

// Function to free process variables
int free_process(process_vars_t* process) {
        
    // Terminate workers
    if (process->workers && process->num_workers > 0) terminate_workers(process->workers, process->num_workers, process->tasks_queue);
    
    // Terminate writer thread
    if (process->writer) terminate_writer(process->writer, process->results_queue);
    
    // Free tasks queue
    if (process->tasks_queue) free_tasks_queue(process->tasks_queue);
    
    // Free results queue
    if (process->results_queue) free_results_queue(process->results_queue); 
    
    // Free file manager
    if (process->file_manager) free_file_manager(process->file_manager);
            
    // Free process variables
    free(process);
    return SUCCESS; 
}

// EOF