#include "../include/task.h"

task_t* create_task(char* data, size_t length, size_t index) {
    
    // Allocate memory for task
    task_t* task = malloc(sizeof(task_t));
    if (!task) {
        
        // Failed to allocate memory for task
        fprintf(stderr, "Error: Failed to allocate memory for task");
        return NULL;
    }
    
    // Initialize task fields
    task->data = data;
    task->length = length;
    task->index = index;
    
    // Successfully created task
    return task;
}

int free_task(task_t* task) {
    
    // Free allocated memory for task
    free(task);
    
    // Successfully freed task
    return SUCCESS;
}
