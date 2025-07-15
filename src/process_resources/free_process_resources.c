#include "../../include/process_resources/free_process_resources.h"

// Function to free process variables
int free_process_resources(process_resources_t* process) {
        
    // Terminate workers
    if (process->workers) free_workers(process->workers, process->num_workers, process->tasks_queue);
    free(process->workers);
    
    // Terminate writer thread
    if (process->writer) free_writer_thread(process->writer, process->results_queue);
    
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