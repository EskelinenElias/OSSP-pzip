#include "../../include/task_manager/yield_task_data.h"

// Function for yielding a task to the task_manager (by main thread)
int yield_task_data(task_manager_t* task_manager, char* data, size_t size) {
    
    // Input validation
    if (!task_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield task: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(task_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield task: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if the task_manager is full and wait for room to become available
    while (task_manager->size == task_manager->capacity) {
        
        // Wait for signal that room is available
        if (pthread_cond_wait(task_manager->room_available, task_manager->lock) != SUCCESS) {
            
            // Failed to wait for signal 
            fprintf(stderr, "Failed to yield task: failed to wait for room\n");
            pthread_mutex_unlock(task_manager->lock); 
            return ERROR;
        }
    }
    
    // Initialize the result corresponding to the task
    if (!(task_manager->results_queue[task_manager->rear] = init_result())) {
        
        // Failed to initialize result
        fprintf(stderr, "Failed to yield task: failed to initialize result\n");
        pthread_mutex_unlock(task_manager->lock);
        return ERROR;
    }
    
    // Initialize a new task and check for errors
    if (!(task_manager->tasks_queue[task_manager->rear] = init_task(data, size, task_manager->results_queue[task_manager->rear]))) {
        
        // Failed to initialize task
        fprintf(stderr, "Failed to yield task: failed to initialize task\n");
        free_result(task_manager->results_queue[task_manager->rear]);
        pthread_mutex_unlock(task_manager->lock);
        return ERROR;
    }
    
    // Increment number of available tasks_queue and size and update rear index
    task_manager->num_available_tasks++; 
    task_manager->size++;
    task_manager->rear = (task_manager->rear + 1) % task_manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(task_manager->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Failed to yield task: failed to signal that a task is available\n");
        pthread_mutex_unlock(task_manager->lock);
        free_task(task_manager->tasks_queue[task_manager->rear]);
        free_result(task_manager->results_queue[task_manager->rear]);
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(task_manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield task: failed to release lock\n");
        free_task(task_manager->tasks_queue[task_manager->rear]);
        free_result(task_manager->results_queue[task_manager->rear]);
        return ERROR;
    }
    
    // Successfully added tasks_queue
    return SUCCESS;
}