#include "../../include/worker/process_tasks.h"

// Thread worker function
void* process_tasks(void* args) {
    
    // Cast arguments
    worker_args_t* worker_args = (worker_args_t*) args;
    tasks_queue_t* tasks_queue = worker_args->tasks_queue;
    results_queue_t* results_queue = worker_args->results_queue; 
    free(args); 
            
    // Process tasks until a termination task is acquired
    task_t* task = NULL; 
    while ((task = claim_task(tasks_queue)) != NULL) {
            
        // Process the current task 
        result_t* result = NULL;
        if (!(result = encode_data(task))) {
            
            // Terminate the thread 
            fprintf(stderr, "Failed to complete task: error occurred during encoding"); 
            return NULL; 
        }; 
                        
        // Yield the result to the task_manager
        if (yield_result(results_queue, result, task->reserved_index) != SUCCESS) {
            
            // Failed to yield result to task_manager
            fprintf(stderr, "Failed to complete task: failed to yield result\n"); 
            free_result(result); 
            return NULL; 
        };
        
        // Free the memory allocated for the task 
        if (free_task(task) != SUCCESS) {
            
            // Failed to free task
            fprintf(stderr, "Failed to complete task: failed to free task\n");
            return NULL;
        }
    }
    
    // Successfully processed tasks
    return NULL; 
}

// EOF