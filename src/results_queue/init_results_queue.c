#include "../../include/results_queue/init_results_queue.h"

// Function to initialize task queue
results_queue_t* init_results_queue(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize results queue: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for results queue structure and check for errors
    results_queue_t* results_queue = (results_queue_t*)malloc(sizeof(results_queue_t)); 
    if (!results_queue) {
        
        // Failed to allocate memory for results queue structure
        fprintf(stderr, "Failed to initialize results queue: failed to allocate memory for results queue\n");
        return NULL; 
    }    

    // Initialize results_queue fields
    results_queue->front = 0;
    results_queue->rear = 0;
    results_queue->size = 0;
    results_queue->capacity = capacity; 
    results_queue->results = NULL; 
    results_queue->room_available = NULL; 
    results_queue->result_available = NULL; 
    results_queue->lock = NULL; 
    
    // Allocate memory for results
    if (!(results_queue->results = (result_t**)malloc(sizeof(result_t*) * capacity))) {
        
        // Failed to allocate memory for results
        fprintf(stderr, "Failed to initialize results queue: failed to allocate memory for results\n");
        free_results_queue(results_queue);
        return NULL; 
    }
    for (size_t i = 0; i < capacity; i++) results_queue->results[i] = NULL;
    
    // Allocate memory for status_flags
    if (!(results_queue->status_flags = (int*)malloc(sizeof(int) * capacity))) {
        
        // Failed to allocate memory for results
        fprintf(stderr, "Failed to initialize results queue: failed to allocate memory for status_flags\n");
        free_results_queue(results_queue);
        return NULL; 
    }
    for (size_t i = 0; i < capacity; i++) results_queue->status_flags[i] = PENDING;
    
    // Allocate memory for mutex lock 
    if (!(results_queue->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for results_queue lock; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize results queue: failed to allocate memory for mutex lock\n");
        free_results_queue(results_queue);
        return NULL; 
        
    }
    
    // Initialize mutex lock 
    if (pthread_mutex_init(results_queue->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock
        fprintf(stderr, "Failed to initialize results queue: failed to initialize mutex lock\n");
        free_results_queue(results_queue);
        return NULL; 
    };
    
    // Allocate memory for condition variable
    if (!(results_queue->room_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize results queue: failed to allocate memory for condition variable\n");
        free_results_queue(results_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(results_queue->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize results queue: failed to initialize condition variable\n");
        free_results_queue(results_queue);
        return NULL;
    }
    
    // Allocate memory for condition variable
    if (!(results_queue->result_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize results queue: failed to allocate memory for condition variable\n");
        free_results_queue(results_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(results_queue->result_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize results queue: failed to initialize condition variable\n");
        free_results_queue(results_queue);
        return NULL;
    }
    
    // Successfully created results queue
    return results_queue; 
}
