#include "../../include/task_manager/claim_task.h"

// Function to claim a task from the task_manager
task_t* claim_task(task_manager_t* task_manager) {
            
    // Input validation
    if (!task_manager || !task_manager->lock || !task_manager->tasks_queue || !task_manager->tasks_available) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim task: invalid input\n");
        return NULL; 
    }
    
    // Acquire lock
    if (pthread_mutex_lock(task_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim task: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if tasks_queue are available 
    while (task_manager->num_available_tasks == 0) {
        
        // Wait for tasks_queue to become available
        if (pthread_cond_wait(task_manager->tasks_available, task_manager->lock) != 0) {
            
            // Failed to wait for tasks_queue to become available
            fprintf(stderr, "Failed to claim task: failed to wait for tasks_queue to become available\n");
            pthread_mutex_unlock(task_manager->lock);
            return NULL;
        }
    }
    
    // Claim a task from the task_manager and check if it's valid
    task_t* task = task_manager->tasks_queue[task_manager->tasks_front]; // NULL pointer represents a termination signal
    task_manager->tasks_queue[task_manager->tasks_front] = NULL; // Set the tasks_queue array slot to NULL (to avoid dangling pointers)
    
    // Increment next task index and decrement number of available tasks_queue
    task_manager->tasks_front = (task_manager->tasks_front + 1) % task_manager->capacity;
    task_manager->num_available_tasks--;
        
    // Release the lock
    if (pthread_mutex_unlock(task_manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim task: failed to release lock\n");
        pthread_mutex_unlock(task_manager->lock); 
        return NULL;
    }
        
    // Return claimed task
    return task;
}