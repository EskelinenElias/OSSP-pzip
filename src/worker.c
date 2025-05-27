#include "../include/worker.h"

// Thread worker function
void* thread_worker(void* arg) {
    
    // Cast arguments
    task_queue_t *task_queue = (task_queue_t *)arg;
    
    // Get the first task from  the task queue
    encoding_task_t task = get_task(task_queue);
    
    // Process tasks until a termination task is acquired
    while (task.input != NULL && task.length > 0) {
        
        // Process the current task 
        if (encode(task.input, task.length, task.output) != SUCCESS) {
            
            // Failed to process task 
            fprintf(stderr, "thread_worker: could not process task\n"); 
            return NULL; // Error
        }; 
        
        // Get a new task from the queue 
        task = get_task(task_queue); 
    }
    return NULL; 
}