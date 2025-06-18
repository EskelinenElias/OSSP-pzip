#include "../include/worker.h"

// Thread worker function
void* thread_worker(void* arg) {
    
    // Cast arguments
    task_manager_t *manager = (task_manager_t *)arg;
        
    // Process tasks until a termination task is acquired
    task_t* task; 
    while ((task = claim_task(manager)) != NULL) {       
        
        // Allocate memory for the output and check for errors
        result_t* result = init_result(ceil((float)task->length / 2));
        if (!result) {
            
            // Failed to allocate memory for the result
            fprintf(stderr, "Terminating thread due to memory allocation failure"); 
            return NULL; 
        }

        // Process the current task 
        if (encode(task->data, task->length, result) != SUCCESS) {
            
            // Terminate the thread 
            fprintf(stderr, "Terminating thread due to memory encoding failure"); 
            free_result(result); 
            return NULL; 
        }; 
        
        // Yield the result to the manager
        if (yield_result(manager, result, task->index) != SUCCESS) {
            
            // Failed to yield result to manager
            fprintf(stderr, "Terminating thread due to result yield failure"); 
            free_result(result); 
            return NULL; 
        };
    }
    return NULL; 
}

// Function to initialize worker threads
pthread_t* init_workers(size_t num_workers, task_manager_t *manager) {
    
    // Allocate memory for worker threads and check for errors
    pthread_t* workers = malloc(num_workers * sizeof(pthread_t));
    if (!workers) {
        
        // Failed to allocate memory for worker threads
        fprintf(stderr, "Failed to allocate memory for worker threads");
        return NULL;
    }
    
    // Initialize worker threads
    for (int i = 0; i < num_workers; i++) { 
                
        // Start a new thread and check for errors
        if (pthread_create(&workers[i], NULL, thread_worker, manager) != 0) {
                        
            // Failed to initialize worker
            terminate_workers(workers, num_workers, manager); 
            return NULL;
        }
    }
    
    // Successfully initialized all worker threads
    return SUCCESS;
}

// Function to terminate all worker threads
int terminate_workers(pthread_t *threads, size_t num_workers, task_manager_t *manager) {
    
    // Input validation
    if (!threads || !manager) return SUCCESS;
    
    // Send termination signal to all threads
    for (int i = 0; i < num_workers; i++) yield_termination_task(manager); 

    // Wait until all threads have terminated
    for (int i = 0; i < num_workers; i++) {
        
        // Wait for thread to return
        pthread_join(threads[i], NULL);
    }
    
    return SUCCESS;
}
