#include "../include/worker.h"

// Thread worker function
void* thread_worker(void* arg) {
    
    // Cast arguments
    task_manager_t *manager = (task_manager_t *)arg;
    
    // Initialize variables
    task_t* task = NULL; 
    result_data_t* result_data = NULL; 
        
    // Process tasks until a termination task is acquired
    while ((task = claim_task(manager)) != NULL) {
        
        // Get task data
        task_data_t task_data = task->data;
        result_t** result = task->result; 
                 
        // Check if the task is an empty task (indicates EOF)
        if (task_data.size == 0) {
            
            // Initiate an empty result and check for errors
            result_data = init_result(0);
            if (!result_data) {
                
                // Failed to allocate memory for the result
                fprintf(stderr, "Terminating thread due to memory allocation failure"); 
                return NULL; 
            }
                        
        } else {
                
            // Allocate memory for the result and check for errors
            result_data = init_result(ceil((float)task_data.size / 2));
            if (!result_data) {
                
                // Failed to allocate memory for the result
                fprintf(stderr, "Terminating thread due to memory allocation failure"); 
                return NULL; 
            }
            
            // Process the current task 
            if (encode(&task_data, result_data) != SUCCESS) {
                
                // Terminate the thread 
                fprintf(stderr, "Terminating thread due to memory encoding failure"); 
                free_result(result_data); 
                return NULL; 
            }; 
        
        }
                
        // Yield the result to the manager
        if (yield_result(manager, result, result_data) != SUCCESS) {
            
            // Failed to yield result to manager
            fprintf(stderr, "Terminating thread due to result yield failure\n"); 
            if (result_data) free_result(result_data); 
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
        fprintf(stderr, "Failed to allocate memory for workers");
        return NULL;
    }
    
    // Initialize worker threads
    for (int i = 0; i < num_workers; i++) { 
                
        // Start a new thread and check for errors
        if (pthread_create(&workers[i], NULL, thread_worker, manager) != 0) {
                        
            // Failed to initialize worker
            fprintf(stderr, "Failed to initialize workers");
            terminate_workers(workers, num_workers, manager); 
            return NULL;
        }
    }
    
    // Successfully initialized all worker threads
    return workers;
}

// Function to terminate all worker threads
int terminate_workers(pthread_t *threads, size_t num_workers, task_manager_t *manager) {
    
    // Input validation
    if (!threads || !manager) {
        
        // Invalid input
        fprintf(stderr, "Invalid input");
        return ERROR;
    };
    
    // Send termination signal to all threads
    for (int i = 0; i < num_workers; i++) {
        
        // Yield a termination task to one worker
        if (yield_termination_task(manager) != SUCCESS) {
            
            // Failed to yield termination task 
            fprintf(stderr, "Failed to yield termination task");
            
            // Force termination 
            // force_termination(manager); 
            break; 
        }
    }
    
    // Wait until all threads have terminated
    for (int i = 0; i < num_workers; i++) {
        
        // Wait for thread to return
        pthread_join(threads[i], NULL);
    }
    
    return SUCCESS;
}
