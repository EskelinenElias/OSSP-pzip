#include "../../include/worker_thread_pool/init_worker_thread.h"

// Function to initialize worker thread
worker_thread_t* init_worker_thread(task_queue_t* task_queue, result_queue_t* result_queue) {
    
    // Input validation
    if (!task_queue || !result_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize worker thread thread: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for worker thread
    worker_thread_t* worker = malloc(sizeof(worker_thread_t));
    if (!worker) {
        
        // Failed to allocate memory for worker thread
        fprintf(stderr, "Failed to initialize worker thread: failed to allocate memory for worker thread\n");
        return NULL;
    }
    
    // Set worker thread fields
    worker->result = NULL; 
    worker->task = NULL; 
    worker->task_queue = task_queue;
    worker->result_queue = result_queue;
                
    // Start a new thread and check for errors
    if (pthread_create(&worker->thread, NULL, worker_thread_main, worker) != 0) {
                    
        // Failed to initialize worker thread
        fprintf(stderr, "Failed to initialize worker thread: failed to create thread\n");
        free(worker);
        return NULL;
    }
    
    // Successfully initialized worker thread
    return worker;
}

// EOF