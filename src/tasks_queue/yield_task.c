#include "../../include/tasks_queue/yield_task.h"

// Function to yield task to tasks queue
int yield_task(tasks_queue_t* tasks_queue, task_data_t* task_data) {
    
    // Input validation
    if (!tasks_queue || !tasks_queue->lock || !tasks_queue->tasks || !tasks_queue->room_available || (task_data && task_data->reserved_index < 0)) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield task: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(tasks_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield task: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if there is room in the queue
    while (tasks_queue->size == tasks_queue->capacity) {
        
        // Wait until there is room in the queue
        if (pthread_cond_wait(tasks_queue->room_available, tasks_queue->lock) != SUCCESS) {
            
            // Failed to wait for until there is room in the queue 
            fprintf(stderr, "Failed to yield task: failed to wait until there is room in the queue\n");
            pthread_mutex_unlock(tasks_queue->lock); 
            return ERROR;
        }
    }
    
    // Add the task to the end of the queue, increment size and update rear index
    size_t task_index = tasks_queue->rear; 
    tasks_queue->tasks[task_index] = task_data; 
    tasks_queue->size++;
    tasks_queue->rear = (tasks_queue->rear + 1) % tasks_queue->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(tasks_queue->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Failed to yield task: failed to signal that a task is available\n");
        tasks_queue->tasks[task_index] = NULL; 
        tasks_queue->rear = task_index; 
        tasks_queue->size--; 
        pthread_mutex_unlock(tasks_queue->lock);
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(tasks_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield task: failed to release lock\n");
        tasks_queue->tasks[task_index] = NULL; 
        tasks_queue->rear = task_index; 
        tasks_queue->size--; 
        pthread_mutex_unlock(tasks_queue->lock);
        return ERROR;
    }
    
    // Successfully added task to tasks queue
    return SUCCESS;
}

// EOF