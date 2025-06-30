#include "../../include/task_manager/task_manager.h"
#include <pthread.h>

// Function to yield a termination task to the manager
int send_termination_signal(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {  // NULL tasks_queue are allowed as they act as termination signals
        
        // Invalid input
        fprintf(stderr, "Error: Manager is NULL\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to send termination signal: failed to acquire lock\n");
        return ERROR;
    }
        
    // Check if the manager is full
    while (manager->size == manager->capacity) {
        
        // Wait until there is space in the manager
        if (pthread_cond_wait(manager->room_available, manager->lock) != 0) {
            
            // Failed to wait for condition variable
            fprintf(stderr, "Failed to send termination signal: failed to wait for room available\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR;
        }
    }
            
    // Add termination task to the manager's task list
    manager->tasks_queue[manager->rear] = NULL;
    
    // Initiate a termination result
    if (!(manager->results_queue[manager->rear] = init_result())) {
        
        // Failed to initialize result
        fprintf(stderr, "Failed to send termination signal: failed to initialize result\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR;
    }
    
    // Set termination result fields
    manager->results_queue[manager->rear]->status = COMPLETED;
    manager->results_queue[manager->rear]->data = NULL; 
    
    // Determine if the result is the next result
    int send_results_available_signal = FALSE; 
    if (manager->rear == manager->results_front) send_results_available_signal = TRUE; 
        
    // Increment size, number of available tasks_queue and results_queue, and update rear index
    manager->size++;
    manager->num_available_tasks++; 
    manager->rear = (manager->rear + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR; 
    }
    
    // Check if the result is the next result 
    if (send_results_available_signal) {
            
        // Signal to the writer that a result is available
        if (pthread_cond_signal(manager->next_result_available) != SUCCESS) {
            
            // Failed to signal condition variable
            fprintf(stderr, "Error: Failed to signal condition variable\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR; 
        }
    }
            
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR;
    }
    
    // Successfully added tasks_queue
    return SUCCESS;
}

// EOF