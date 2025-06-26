#include "../../include/task_manager/task.h"

task_t* init_task(char* data, size_t size, result_t* result) {
    
    // Allocate memory for task
    task_t* task = malloc(sizeof(task_t));
    if (!task) {
        
        // Failed to allocate memory for task
        fprintf(stderr, "Failed to initialize task: Failed to allocate memory for task");
        return NULL;
    }
    
    // Initialize task fields
    task->task_data.data = data;
    task->task_data.size = size;
    task->result = result; 
    
    // Successfully created task
    return task;
}

void* free_task(task_t* task) {
        
    // Free allocated memory for task
    free(task);
    
    // Successfully freed task
    return NULL;
}
