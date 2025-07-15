#include "../../include/worker/free_worker_thread.h"

// Function to free worker thread
int free_worker_thread(worker_thread_t* worker) {
    
    // Input validation
    if (!worker) {
        
        // Invalid input
        fprintf(stderr, "Failed to free worker thread: invalid input\n");
        return ERROR;
    }
    
    // Check if the worker thread is still running 
    if (worker->thread) {
        
        // Worker thread is still running
        fprintf(stderr, "Failed to free worker thread: worker thread is still running\n");
        return ERROR;
    }
    
    // Free memory allocated for worker thread resources
    if (worker->task) free_task(worker->task); 
    if (worker->result) free_result(worker->result);
    
    // Free memory allocated for worker thread
    free(worker);
    
    // Successfully freed worker thread
    return SUCCESS;
}

// EOF