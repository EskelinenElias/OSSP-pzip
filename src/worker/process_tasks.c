#include "../../include/worker/process_tasks.h"

// Thread worker function
void* process_tasks(void* args) {

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
            if (worker->task) free_task(worker->task);
            worker->task = NULL; 
            if (worker->result) free_result(worker->result);
            worker->result = NULL;
            return NULL; 
        }; 
                        
        // Yield the result to the task_manager
        size_t reserved_index = worker->task->reserved_index; 
        if (yield_result(results_queue, worker->result, reserved_index) != SUCCESS) {
            
            // Failed to yield result to task_manager
            fprintf(stderr, "Failed to complete task: failed to yield result\n"); 
            if (worker->task) free_task(worker->task);
            worker->task = NULL; 
            if (worker->result) free_result(worker->result);
            worker->result = NULL;
            return NULL; 
        };
        worker->result = NULL; // Reset result pointer
        
        // Free the memory allocated for the task 
        if (free_task(worker->task) != SUCCESS) {
            
            // Failed to free task
            fprintf(stderr, "Failed to complete task: failed to free task\n");
            if (worker->task) free_task(worker->task);
            worker->task = NULL; 
            if (worker->result) free_result(worker->result);
            worker->result = NULL;
            return NULL;
        }
        worker->task = NULL; // Reset task pointer 
    }
    
    // Free worker resources
    if (worker->task) free_task(worker->task);
    worker->task = NULL; 
    if (worker->result) free_result(worker->result);
    worker->result = NULL;    
    
    // Successfully processed tasks
    return NULL; 
}

// EOF