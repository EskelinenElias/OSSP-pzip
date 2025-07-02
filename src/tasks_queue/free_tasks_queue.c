#include "../../include/tasks_queue/free_tasks_queue.h"

// Function to free the tasks queue
int free_tasks_queue(tasks_queue_t* tasks_queue) {
    
    // Input validation
    if (!tasks_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to free tasks queue: invalid input\n");
        return ERROR; 
    }
    
    // Check if mutex lock exists
    if (tasks_queue->lock) {
        
        // Destroy mutex lock for tasks_queue
        pthread_mutex_destroy(tasks_queue->lock);
        free(tasks_queue->lock);
    }
    
    // Check if condition variable for room availability exists
    if (tasks_queue->room_available) {
        
        // Destroy condition variable for room availability
        pthread_cond_destroy(tasks_queue->room_available);
        free(tasks_queue->room_available);
    }   
    
    // Check if condition variable for tasks_queue availability exists
    if (tasks_queue->tasks_available) {
        
        // Destroy condition variable for tasks_queue availability
        pthread_cond_destroy(tasks_queue->tasks_available);
        free(tasks_queue->tasks_available);
    }
    
    // Free any remaining tasks 
    for (size_t i = 0; i < tasks_queue->capacity; i++) {
        
        // Check if the task still exists
        if (!tasks_queue->tasks[i]) continue; 
        
        // Free the task 
        if (free_task(tasks_queue->tasks[i]) != SUCCESS) {
             
            // Failed to free task 
            fprintf(stderr, "Failed to free tasks queue: failed to free remaining tasks\n"); 
        }
    }
    
    // Free memory allocated for tasks
    if (tasks_queue->tasks) free(tasks_queue->tasks);
    
    // Free memory allocated for tasks queue structure
    free(tasks_queue); 
    
    // Successfully freed tasks queue
    return SUCCESS; 
}

// EOF