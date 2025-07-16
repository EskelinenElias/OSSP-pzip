#include "../../include/task_queue/yield_NULL_task.h"

// Function to yield NULL task to task queue
int yield_NULL_task(task_queue_t* task_queue) {
    
    // Input validation
    if (!task_queue || !task_queue->lock || !task_queue->tasks || !task_queue->room_available) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield NULL task: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(task_queue->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield NULL task: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if there is room in the queue
    while (task_queue->size == task_queue->capacity) {
        
        // Wait until room becomes available (as worker threads claim tasks)
        if (pthread_cond_wait(task_queue->room_available, task_queue->lock) != SUCCESS) {
            
            // Failed to wait for room to become available
            fprintf(stderr, "Failed to yield NULL task: failed to wait for room to become available\n");
            pthread_mutex_unlock(task_queue->lock); 
            return ERROR;
        }
    }
    
    // Check that the spot is actually empty
    if (task_queue->tasks[task_queue->tail]) {
        
        // Failed to yield NULL task: spot is not empty
        fprintf(stderr, "Failed to yield NULL task: something went wrong\n");
        pthread_mutex_unlock(task_queue->lock); 
        return ERROR;
    }
    
    // Add NULL task to the end of the queue
    size_t task_index = task_queue->tail; 
    task_queue->tasks[task_index] = NULL; 
    
    // Increment queue size and update tail index
    task_queue->size++;
    task_queue->tail = (task_queue->tail + 1) % task_queue->capacity;
    
    // Signal to worker threads that task is available
    if (pthread_cond_signal(task_queue->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Failed to yield NULL task: failed to signal that task is available\n");
        task_queue->tail = task_index; 
        task_queue->size--; 
        pthread_mutex_unlock(task_queue->lock);
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(task_queue->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield NULL task: failed to release lock\n");
        task_queue->tail = task_index; 
        task_queue->size--; 
        pthread_mutex_unlock(task_queue->lock);
        return ERROR;
    }
    
    // Successfully yield NULL task to task queue
    return SUCCESS;
}

// EOF