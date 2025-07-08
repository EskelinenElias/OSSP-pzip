#include "../../include/worker/process_tasks.h"

// Function to initialize worker resources
worker_resources_t* init_worker_resources() {
    
    // Allocate memory for the resources structure
    worker_resources_t* resources = calloc(1, sizeof(worker_resources_t));
    if (!resources) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize worker resources: failed to allocate memory for resources\n");
        return NULL;
    }

    // Successfully initialized resources
    return resources;
}

// Function to free worker resources
int free_worker_resources(worker_resources_t* resources) {
    
    // Input validation
    if (!resources) {
        
        // Invalid input
        fprintf(stderr, "Failed to cleanup resources: invalid resources\n");
        return FAILURE;
    }
    
    // Free memory allocated for the resources
    if (resources->task) free(resources->task); 
    if (resources->result) free(resources->result); 
    
    // Free memory allocated for the resources structure
    free(resources);
    
    // Successfully cleaned up resources
    return SUCCESS;
}

// Thread worker function
void* process_tasks(void* args) {
    
    // Input validation
    if (!args) {
        
        // Invalid input
        fprintf(stderr, "Failed to process tasks: invalid input\n");
        return NULL;
    }
    
    // Parse arguments
    worker_args_t* worker_args = (worker_args_t*) args;
    if (!worker_args->tasks_queue || !worker_args->results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to process tasks: missing worker arguments\n");
        free(worker_args); 
        return NULL;
    }
    tasks_queue_t* tasks_queue = worker_args->tasks_queue;
    results_queue_t* results_queue = worker_args->results_queue; 
    free(worker_args); 
    
    // Initialize worker resources
    worker_resources_t* resources = init_worker_resources(); 
    if (!resources) {
        
        // Failed to initialize worker resources
        fprintf(stderr, "Failed to process tasks: failed to initialize worker resources\n");
        return NULL;
    }
            
    // Process tasks until a termination task is acquired
    while ((resources->task = claim_task(tasks_queue)) != NULL) {
            
        // Process the current task 
        if (!(resources->result = encode_data(resources->task))) {
            
            // Terminate the thread 
            fprintf(stderr, "Failed to complete task: error occurred during encoding"); 
            free_worker_resources(resources);
            return NULL; 
        }; 
                        
        // Yield the result to the task_manager
        size_t reserved_index = resources->task->reserved_index; 
        if (yield_result(results_queue, resources->result, reserved_index) != SUCCESS) {
            
            // Failed to yield result to task_manager
            fprintf(stderr, "Failed to complete task: failed to yield result\n"); 
            free_worker_resources(resources);
            return NULL; 
        };
        resources->result = NULL; // Reset result pointer
        
        // Free the memory allocated for the task 
        if (free_task(resources->task) != SUCCESS) {
            
            // Failed to free task
            fprintf(stderr, "Failed to complete task: failed to free task\n");
            free_worker_resources(resources);
            return NULL;
        }
        resources->task = NULL; // Reset task pointer 
    }
    
    // Free worker resources
    free_worker_resources(resources);
    
    // Successfully processed tasks
    return NULL; 
}

// EOF