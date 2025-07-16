#include "../../include/task_queue/free_task_queue.h"

// Function to free the task queue
int free_task_queue(task_queue_t* task_queue) {
    
    // Input validation
    if (!task_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free task queue: invalid input\n");
        return ERROR; 
    }
    
    // Check if mutex lock exists
    if (task_queue->lock) {
        
        // Destroy mutex lock
        pthread_mutex_destroy(task_queue->lock);
        free(task_queue->lock);
    }
    
    // Check if condition variable for room availability exists
    if (task_queue->room_available) {
        
        // Destroy condition variable
        pthread_cond_destroy(task_queue->room_available);
        free(task_queue->room_available);
    }   
    
    // Check if condition variable for task availability exists
    if (task_queue->tasks_available) {
        
        // Destroy condition variable
        pthread_cond_destroy(task_queue->tasks_available);
        free(task_queue->tasks_available);
    }
    
    // Free any remaining tasks
    for (size_t i = 0; i < task_queue->capacity; i++) {
        
        // Check if the task still exists
        if (!task_queue->tasks[i]) continue; 
        
        // Free the task 
        if (free_encoding_task(task_queue->tasks[i]) != SUCCESS) {
             
            // Failed to free task 
            fprintf(stderr, "Failed to free task queue: failed to free task\n"); 
        }
    }
    
    // Free memory allocated for tasks
    if (task_queue->tasks) free(task_queue->tasks);
    
    // Free memory allocated for task queue
    free(task_queue); 
    
    // Successfully freed task queue
    return SUCCESS; 
}

// EOF