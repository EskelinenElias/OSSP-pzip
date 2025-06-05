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

// Function to initialize worker threads
int init_workers(pthread_t *threads, size_t num_workers, task_queue_t *queue) {
    for (int i = 0; i < num_workers; i++) { 
    
        // Initialize a thread
        if (pthread_create(&threads[i], NULL, thread_worker, &queue) != 0) {

            // Cleanup already created threads
            for (int j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }
            return FAILURE;
        }
    }

    return SUCCESS; // Success
}

// Function to terminate all worker threads
int terminate_workers(pthread_t *threads, size_t num_workers, task_queue_t *queue) {
    
    // Send termination signal to all threads
    for (int i = 0; i < num_workers; i++) {
        
        // Add termination task to the queue
        encoding_task_t termination_task = get_termination_task(); 
        add_task(queue, termination_task); 
    }

    // Wait until all threads have terminated
    for (int i = 0; i < num_workers; i++) {
        
        // Wait for thread to return
        pthread_join(threads[i], NULL);
    }
    
    return SUCCESS; // Success
}
