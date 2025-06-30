#include "../../include/task_manager/free_task_manager.h"

// Function to free the task manager
void* free_task_manager(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to free manager: Invalid input\n");
        return NULL; 
    }
    
    // Check if mutex lock exists
    if (manager->lock) {
        
        // Destroy mutex lock for tasks_queue
        pthread_mutex_destroy(manager->lock);
        free(manager->lock);
    }
    
    // Check if condition variable for room availability exists
    if (manager->room_available) {
        
        // Destroy condition variable for room availability
        pthread_cond_destroy(manager->room_available);
        free(manager->room_available);
    }   
    
    // Check if condition variable for tasks_queue availability exists
    if (manager->tasks_available) {
        
        // Destroy condition variable for tasks_queue availability
        pthread_cond_destroy(manager->tasks_available);
        free(manager->tasks_available);
    }
    
    // Check if condition variable for results_queue availability exists
    if (manager->next_result_available) {
        
        // Destroy condition variable for results_queue availability
        pthread_cond_destroy(manager->next_result_available);
        free(manager->next_result_available);
    }
    
    // Check if condition variable for taskscompletion exists
    if (manager->tasks_completed) {
        
        // Destroy condition variable for completion
        pthread_cond_destroy(manager->tasks_completed);
        free(manager->tasks_completed);
    }
    
    // Free memory allocated for tasks_queue and results_queue arrays
    if (manager->tasks_queue) free(manager->tasks_queue);
    if (manager->results_queue) free(manager->results_queue);
    
    // Free memory allocated for manager structure
    free(manager); 
    
    // Successfully freed manager
    return NULL; 
}

// Function to wait for all tasks_queue to complete
int wait_for_completion(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to wait for completion: Invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if(pthread_mutex_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to wait for completion: failed to acquire lock\n");
        return ERROR;
    }
    
    for (int i = 0; i < manager->capacity; i++) {
        if (manager->results_queue[i]) {
            if (manager->results_queue[i]->status == COMPLETED && manager->results_queue[i]->data == NULL) {
                fprintf(stderr, "Result %d is TERMINATION\n", i); 
            } else if (manager->results_queue[i]->status == COMPLETED && manager->results_queue[i]->data != NULL) {
                fprintf(stderr, "Result %d is COMPLETED\n", i); 
            }
        }
    }
    
    // Wait for all tasks_queue to complete
    while (manager->size > 0) {
        fprintf(stderr, "Waiting for tasks_queue to complete (%zu tasks_queue left)...\n", manager->size);
        
        // Wait for tasks_queue to become available
        if (pthread_cond_wait(manager->tasks_completed, manager->lock) != SUCCESS) {
            
            // Failed to wait for condition variable
            fprintf(stderr, "Failed to wait for completion: error occurred while waiting for tasks_queue\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR;
        }
    }
    
    // Release lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to wait for completion: failed to release lock\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR;
    }
    
    // Successfully waited for completion
    return SUCCESS;
}