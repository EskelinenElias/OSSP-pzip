#include "../../include/worker/init_worker_group.h"

// Function to initialize worker group
worker_group_t* init_worker_group(size_t num_workers, tasks_queue_t* tasks_queue, results_queue_t* results_queue) {
    
    // Input validation
    if (!tasks_queue || !results_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize worker group: invalid input\n");
        return NULL; 
    }
    
    // Allocate memory for worker group
    worker_group_t* worker_group = malloc(sizeof(worker_group_t));
    if (!worker_group) {
        
        // Failed to allocate memory for worker group
        fprintf(stderr, "Failed to initialize worker group: failed to allocate memory for worker group\n");
        return NULL;
    }
    
    // Allocate memory for workers
    worker_group->workers = calloc(num_workers, sizeof(worker_thread_t*));
    if (!worker_group->workers) {
        
        // Failed to allocate memory for workers
        fprintf(stderr, "Failed to initialize worker group: failed to allocate memory for workers\n");
        free(worker_group);
        return NULL;
    }
    
    // Set worker group fields
    worker_group->num_workers = num_workers;
    
    // Initialize workers
    for (size_t i = 0; i < num_workers; i++) {
        
        // Initialize worker
        worker_group->workers[i] = init_worker_thread(tasks_queue, results_queue);
        if (!worker_group->workers[i]) {
            
            // Failed to initialize worker
            fprintf(stderr, "Failed to initialize worker group: failed to initialize worker %zu\n", i);
            free_worker_group(worker_group, tasks_queue);
            return NULL;
        }
    }
    
    // Successfully initialized worker group
    return worker_group;
}

// EOF