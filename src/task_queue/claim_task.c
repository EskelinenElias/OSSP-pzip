#include "../../include/task_queue/claim_task.h"

// Function to claim task from task queue
encoding_task_t* claim_task(task_queue_t* task_queue) {
            
    // Input validation
    if (!task_queue || !task_queue->lock || !task_queue->tasks || !task_queue->tasks_available) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim task: invalid input\n");
        return NULL; 
    }
    
    // Acquire lock
    if (pthread_mutex_lock(task_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim task: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if there are any task in the queue
    while (task_queue->size == 0) {
        
        // Wait for task to become available
        if (pthread_cond_wait(task_queue->tasks_available, task_queue->lock) != 0) {
            
            // Failed to wait for task to become available
            fprintf(stderr, "Failed to claim task: failed to wait for task to become available\n");
            pthread_mutex_unlock(task_queue->lock);
            return NULL;
        }
    }
    
    // Claim a task from the queue and set the spot in the queue to NULL
    size_t task_index = task_queue->head; 
    encoding_task_t* task = task_queue->tasks[task_index];
    task_queue->tasks[task_index] = NULL; 
    
    // Decrement task queue size and update head index
    task_queue->head = (task_queue->head + 1) % task_queue->capacity;
    task_queue->size--;
    
    // Check if room just became available
    if (task_queue->size == task_queue->capacity - 1) {
        
        // Signal that there is room in the task queue
        if (pthread_cond_signal(task_queue->room_available) != 0) {
            
            // Failed to signal that there is room in the task queue
            fprintf(stderr, "Failed to claim task: failed to signal that there is room available in the task queue\n"); 
            free_encoding_task(task); 
            pthread_mutex_unlock(task_queue->lock); 
            return NULL;
        }
    }
        
    // Release the lock
    if (pthread_mutex_unlock(task_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim task: failed to release lock\n");
        free_encoding_task(task); 
        pthread_mutex_unlock(task_queue->lock); 
        return NULL;
    }
        
    // Successfully claimed task 
    return task;
}

// EOF