#include "../../include/task_queue/yield_task.h"

// Function to yield task to task queue
int yield_task(task_queue_t* task_queue, task_t* task) {
    
    // Input validation
    if (!task_queue || !task_queue->lock || !task_queue->tasks || !task_queue->room_available || (task && task->reserved_index < 0)) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield task: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(task_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield task: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if there is room in the queue
    while (task_queue->size == task_queue->capacity) {
        
        // Wait until there is room in the queue
        if (pthread_cond_wait(task_queue->room_available, task_queue->lock) != SUCCESS) {
            
            // Failed to wait for until there is room in the queue 
            fprintf(stderr, "Failed to yield task: failed to wait until there is room in the queue\n");
            pthread_mutex_unlock(task_queue->lock); 
            return ERROR;
        }
    }
    
    // Add the task to the end of the queue, increment size and update tail index
    size_t task_index = task_queue->tail; 
    task_queue->tasks[task_index] = task; 
    task_queue->size++;
    task_queue->tail = (task_queue->tail + 1) % task_queue->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(task_queue->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Failed to yield task: failed to signal that a task is available\n");
        task_queue->tasks[task_index] = NULL; 
        task_queue->tail = task_index; 
        task_queue->size--; 
        pthread_mutex_unlock(task_queue->lock);
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(task_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield task: failed to release lock\n");
        task_queue->tasks[task_index] = NULL; 
        task_queue->tail = task_index; 
        task_queue->size--; 
        pthread_mutex_unlock(task_queue->lock);
        return ERROR;
    }
    
    // Successfully added task to task queue
    return SUCCESS;
}

// EOF