#include "../../include/task_queue/yield_task.h"

// Function to yield task to task queue
int yield_task(task_queue_t* task_queue, char* data, size_t size) {
    
    // Input validation
    if (!task_queue || !task_queue->lock || !task_queue->tasks || !task_queue->room_available || size < 0) {
        
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
        
        // Wait until room becomes available (as worker threads claim tasks)
        if (pthread_cond_wait(task_queue->room_available, task_queue->lock) != SUCCESS) {
            
            // Failed to wait for room to become available
            fprintf(stderr, "Failed to yield task: failed to wait for room to become available\n");
            pthread_mutex_unlock(task_queue->lock); 
            return ERROR;
        }
    }
    
    // Initialize a task 
    task_t* task = init_task(data, size, task_queue->result_queue); 
    if (!task) {
        
        // Failed to initialize task
        fprintf(stderr, "Failed to yield task: failed to initialize task\n");
        pthread_mutex_unlock(task_queue->lock); 
        return ERROR;
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