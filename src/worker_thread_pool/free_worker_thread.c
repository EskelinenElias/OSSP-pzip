#include "../../include/worker_thread_pool/free_worker_thread.h"

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
    if (worker->encoding_task) free_task(worker->encoding_task); 
    if (worker->encoding_result) free_encoding_result(worker->encoding_result);
    
    // Free memory allocated for worker thread
    free(worker);
    
    // Successfully freed worker thread
    return SUCCESS;
}

// EOF