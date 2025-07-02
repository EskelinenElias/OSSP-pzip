#include "../../include/tasks_queue/claim_task.h"

// Function to claim task from tasks queue
task_t* claim_task(tasks_queue_t* tasks_queue) {
            
    // Input validation
    if (!tasks_queue || !tasks_queue->lock || !tasks_queue->tasks || !tasks_queue->tasks_available) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim task: invalid input\n");
        return NULL; 
    }
    
    // Acquire lock
    if (pthread_mutex_lock(tasks_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim task: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if there are any tasks in the queue
    while (tasks_queue->size == 0) {
        
        // Wait for tasks to become available
        if (pthread_cond_wait(tasks_queue->tasks_available, tasks_queue->lock) != 0) {
            
            // Failed to wait for tasks to become available
            fprintf(stderr, "Failed to claim task: failed to wait for tasks to become available\n");
            pthread_mutex_unlock(tasks_queue->lock);
            return NULL;
        }
    }
    
    // Claim a task from the queue and set the spot in the queue to NULL
    size_t task_index = tasks_queue->front; 
    task_t* task = tasks_queue->tasks[task_index];
    tasks_queue->tasks[task_index] = NULL; 
    
    // Decrement tasks queue size and update front index
    tasks_queue->front = (tasks_queue->front + 1) % tasks_queue->capacity;
    tasks_queue->size--;
    
    // Check if room just became available
    if (tasks_queue->size == tasks_queue->capacity - 1) {
        
        // Signal that there is room in the tasks queue
        if (pthread_cond_signal(tasks_queue->room_available) != 0) {
            
            // Failed to signal that there is room in the tasks queue
            fprintf(stderr, "Failed to claim task: failed to signal that there is room available in the tasks queue\n"); 
            free_task(task); 
            pthread_mutex_unlock(tasks_queue->lock); 
            return NULL;
        }
    }
        
    // Release the lock
    if (pthread_mutex_unlock(tasks_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim task: failed to release lock\n");
        free_task(task); 
        pthread_mutex_unlock(tasks_queue->lock); 
        return NULL;
    }
        
    // Successfully claimed task 
    return task;
}

// EOF