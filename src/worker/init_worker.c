#include "../../include/worker/init_worker.h"

// Function to initialize worker threads
pthread_t* init_worker(task_manager_t* task_manager) {
    
    // Allocate memory for worker threads and check for errors
    pthread_t* worker = (pthread_t*)malloc(sizeof(pthread_t));
    if (!worker) {
        
        // Failed to allocate memory for worker threads
        fprintf(stderr, "Failed to initialize worker: failed to allocate memory for worker\n");
        return NULL;
    }
    
    // Allocate memory for worker arguments
    worker_args_t* worker_args = (worker_args_t*)malloc(sizeof(worker_args_t));
    if (!worker_args) {
        
        // Failed to allocate memory for worker arguments
        fprintf(stderr, "Failed to initialize worker: failed to allocate memory for worker arguments\n");
        free(worker);
        return NULL;
    }
    
    // Set worker arguments
    worker_args->task_manager = task_manager;
                
    // Start a new thread and check for errors
    if (pthread_create(worker, NULL, do_work, worker_args) != 0) {
                    
        // Failed to initialize worker
        fprintf(stderr, "Failed to initialize worker: failed to create thread\n");
        free(worker_args);
        free(worker);
        return NULL;
    }
    
    // Successfully initialized all worker threads
    return worker;
}