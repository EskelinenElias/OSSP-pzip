#include "../../include/encoding_task/init_encoding_task.h"

// Function to initialize encoding task
encoding_task_t* init_encoding_task(char* data, size_t size, result_queue_t* result_queue) {
    
    // Input validation
    if (!result_queue || size < 0 || (data && size == 0) || (!data && size > 0)) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize encoding task: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for encoding task
    encoding_task_t* encoding_task = (encoding_task_t*)malloc(sizeof(encoding_task_t));
    if (!encoding_task) {
        
        // Failed to allocate memory for encoding task
        fprintf(stderr, "Failed to initialize encoding task: failed to allocate memory for encoding_task\n");
        return NULL;
    }
    
    // Reserve a spot for the encoding task in the result queue
    size_t reserved_index = reserve_spot(result_queue);
    if (reserved_index < 0) {
        
        // Failed to reserve spot in result queue
        fprintf(stderr, "Failed to initialize encoding task: failed to reserve spot in result queue\n");
        free(encoding_task);
        return NULL;
    }
    
    // Initialize encoding task fields
    encoding_task->data = data;
    encoding_task->size = size;
    encoding_task->reserved_index = reserved_index; 
    
    // Successfully created encoding task
    return encoding_task;
}

// EOF