#include "../include/task.h"

task_t* init_task(char* data, size_t size, result_t** result, int* status) {
    
    // Allocate memory for task
    task_t* task = malloc(sizeof(task_t));
    if (!task) {
        
        // Failed to allocate memory for task
        fprintf(stderr, "Error: Failed to allocate memory for task");
        return NULL;
    }
    
    // Initialize task fields
    task->data = data;
    task->size = size;
    task->result = result;
    task->status = status;
    
    // Successfully created task
    return task;
}

int free_task(task_t* task) {
        
    // Free allocated memory for task
    free(task);
    
    // Successfully freed task
    return SUCCESS;
}
