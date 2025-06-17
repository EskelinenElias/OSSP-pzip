#include "../include/worker.h"
#include <stdatomic.h>

// Thread worker function
void* thread_worker(void* arg) {
    
    // Cast arguments
    task_queue_t *queue = (task_queue_t *)arg;
        
    // Process tasks until a termination task is acquired
    encoding_task_t* task = get_task(queue); 
    while (task != NULL) {       
        
        // Allocate memory for the output
        task->output = create_encoded_data(ceil((float)task->length / 2));
        if (task->output == NULL) {
            
            // Memory allocation failed; mark the task as failed
            atomic_store(&task->status, FAILED);
            
            // Terminate the thread 
            return NULL; 
        }
        
        // Process the current task 
        if (encode(task->input, task->length, task->output) != SUCCESS) {
            
            // Encoding failed; mark the task as failed
            task->status = FAILED;
            
            // Terminate the thread 
            return NULL; 
        }; 
        
        // Encoding was successfully completed; mark the task as completed
        atomic_store(&task->status, COMPLETED);
        
        // Get a new task from the queue 
        task = get_task(queue); 
    }
    return NULL; 
}

// Function to initialize worker threads
int init_workers(pthread_t *threads, size_t num_workers, task_queue_t *queue) {
    
    // Initialize worker threads
    for (int i = 0; i < num_workers; i++) { 
                
        // Start a new thread and check for errors
        if (pthread_create(&threads[i], NULL, thread_worker, queue) != 0) {
                        
            // Error; cleanup already created threads
            for (int j = 0; j < i; j++) pthread_join(threads[j], NULL);
            return FAILURE;
        }
    }
    
    // Successfully initialized all worker threads
    return SUCCESS;
}

// Function to terminate all worker threads
int terminate_workers(pthread_t *threads, size_t num_workers, task_queue_t *queue) {
    
    // Input validation
    if (!threads || !queue) return SUCCESS;
    
    // Send termination signal to all threads
    for (int i = 0; i < queue->capacity; i++) {
        
        // Add termination task to the queue
        add_task(queue, NULL); 
    }

    // Wait until all threads have terminated
    for (int i = 0; i < num_workers; i++) {
        
        // Wait for thread to return
        pthread_join(threads[i], NULL);
    }
    
    return SUCCESS; // Success
}
