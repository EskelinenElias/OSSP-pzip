#include "../../include/task/init_task.h"

// Function to initialize task
task_t* init_task(char* data, size_t size, results_queue_t* results_queue) {
    
    // Input validation
    if (!results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize task: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for task
    task_t* task = (task_t*)malloc(sizeof(task_t));
    if (!task) {
        
        // Failed to allocate memory for task
        fprintf(stderr, "Failed to initialize task: failed to allocate memory for task\n");
        return NULL;
    }
    
    // Reserve a spot for the task in the results queue
    size_t reserved_index = reserve_spot(results_queue);
    if (reserved_index < 0) {
        
        // Failed to reserve spot in results queue
        fprintf(stderr, "Failed to initialize task: failed to reserve spot in results queue\n");
        free(task);
        return NULL;
    }
    
    // Initialize task fields
    task->data = data;
    task->size = size;
    task->reserved_index = reserved_index; 
    
    // Successfully created task
    return task;
}

// EOF