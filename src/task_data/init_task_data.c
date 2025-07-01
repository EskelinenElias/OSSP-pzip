#include "../../include/task_data/init_task_data.h"

// Function to initialize task data
task_data_t* init_task_data(char* data, size_t size, size_t reserved_index) {
    
    // Allocate memory for task
    task_data_t* task_data = (task_data_t*)malloc(sizeof(task_data_t));
    if (!task_data) {
        
        // Failed to allocate memory for task
        fprintf(stderr, "Failed to initialize task data: failed to allocate memory for task data\n");
        return NULL;
    }
    
    // Initialize task fields
    task_data->data = data;
    task_data->size = size;
    task_data->reserved_index = reserved_index; 
    
    // Successfully created task
    return task_data;
}

// EOF