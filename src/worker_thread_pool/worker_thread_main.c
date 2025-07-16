#include "../../include/worker_thread_pool/worker_thread_main.h"

// Function to clear worker thread resources
int clear_worker_thread_resources(worker_thread_t* worker) {
    
    // Input validation
    if (!worker) {
        
        // Invalid input
        fprintf(stderr, "Failed to clear worker thread resources: invalid input\n");
        return ERROR; 
    }
    
    // Free worker thread task
    if (worker->encoding_task && free_encoding_task(worker->encoding_task) != SUCCESS) {
        
        // Failed to free worker thread task
        fprintf(stderr, "Failed to clear worker thread resources: failed to free encoding task\n");
        return ERROR; 
    }
    
    // Reset worker thread task
    worker->encoding_task = NULL;
    
    // Free worker thread encoded data
    if (worker->encoding_result && free_encoding_result(worker->encoding_result) != SUCCESS) {
        
        // Failed to free worker thread encoding_result
        fprintf(stderr, "Failed to clear worker thread resources: failed to free encoded data\n");
        return ERROR; 
    }
    
    // Reset worker thread encoding_result
    worker->encoding_result = NULL;
    
    // Successfully cleared worker thread resources
    return SUCCESS; 
}

// Worker thread main function (worker threads process task from task queue)
void* worker_thread_main(void* args) {

    // Input validation
    worker_thread_t* worker = (worker_thread_t*) args;
    if (!worker || !worker->task_queue || !worker->task_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to process task: missing worker arguments\n");
        return NULL;
    }
    
    // Setup shorthands
    task_queue_t* task_queue = worker->task_queue;
    result_queue_t* result_queue = worker->result_queue; 
            
    // Start processing tasks
    while ((worker->encoding_task = claim_task(task_queue)) != NULL) {
        
        // Get encoding task data
        char* data = worker->encoding_task->data;
        size_t size = worker->encoding_task->size;
        size_t reserved_index = worker->encoding_task->reserved_index; 
        
        // Free the memory allocated for the task 
        if (free_encoding_task(worker->encoding_task) != SUCCESS) {
            
            // Failed to free task
            fprintf(stderr, "Failed to complete task: failed to free encoding task\n");
            clear_worker_thread_resources(worker); 
            return NULL;
        }
        worker->encoding_task = NULL; // Reset encoding task pointer  
        
        // Process the current task 
        if (!(worker->encoding_result = encode_data(data, size))) {
            
            // Terminate the thread 
            fprintf(stderr, "Failed to complete task: error occurred during encoding"); 
            clear_worker_thread_resources(worker); 
            return NULL; 
        }; 
                        
        // Yield the encoding_result to the task_manager
        if (yield_result(result_queue, worker->encoding_result, reserved_index) != SUCCESS) {
            
            // Failed to yield encoding_result to task_manager
            fprintf(stderr, "Failed to complete task: failed to yield encoded data\n"); 
            clear_worker_thread_resources(worker); 
            return NULL; 
        };
        worker->encoding_result = NULL; // Reset encoded data pointer
    }
    
    // Clear worker thread resources
    clear_worker_thread_resources(worker); 
    
    // Successfully processed task
    return NULL; 
}

// EOF