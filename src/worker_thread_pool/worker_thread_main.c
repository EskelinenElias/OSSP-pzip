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
    if (worker->task && free_task(worker->task) != SUCCESS) {
        
        // Failed to free worker thread task
        fprintf(stderr, "Failed to clear worker thread resources: failed to free worker thread task\n");
        return ERROR; 
    }
    
    // Reset worker thread task
    worker->task = NULL;
    
    // Free worker thread result
    if (worker->result && free_result(worker->result) != SUCCESS) {
        
        // Failed to free worker thread result
        fprintf(stderr, "Failed to clear worker thread resources: failed to free worker thread result\n");
        return ERROR; 
    }
    
    // Reset worker thread result
    worker->result = NULL;
    
    // Successfully cleared worker thread resources
    return SUCCESS; 
}

// Worker thread main function (worker threads process tasks from tasks queue)
void* worker_thread_main(void* args) {

    // Input validation
    worker_thread_t* worker = (worker_thread_t*) args;
    if (!worker || !worker->tasks_queue || !worker->results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to process tasks: missing worker arguments\n");
        return NULL;
    }
    
    // Setup shorthands
    tasks_queue_t* tasks_queue = worker->tasks_queue;
    results_queue_t* results_queue = worker->results_queue; 
            
    // Process tasks until a termination task is acquired
    while ((worker->task = claim_task(tasks_queue)) != NULL) {
            
        // Process the current task 
        if (!(worker->result = encode_data(worker->task))) {
            
            // Terminate the thread 
            fprintf(stderr, "Failed to complete task: error occurred during encoding"); 
            clear_worker_thread_resources(worker); 
            return NULL; 
        }; 
                        
        // Yield the result to the task_manager
        size_t reserved_index = worker->task->reserved_index; 
        if (yield_result(results_queue, worker->result, reserved_index) != SUCCESS) {
            
            // Failed to yield result to task_manager
            fprintf(stderr, "Failed to complete task: failed to yield result\n"); 
            clear_worker_thread_resources(worker); 
            return NULL; 
        };
        worker->result = NULL; // Reset result pointer
        
        // Free the memory allocated for the task 
        if (free_task(worker->task) != SUCCESS) {
            
            // Failed to free task
            fprintf(stderr, "Failed to complete task: failed to free task\n");
            clear_worker_thread_resources(worker); 
            return NULL;
        }
        worker->task = NULL; // Reset task pointer 
    }
    
    // Clear worker thread resources
    clear_worker_thread_resources(worker); 
    
    // Successfully processed tasks
    return NULL; 
}

// EOF