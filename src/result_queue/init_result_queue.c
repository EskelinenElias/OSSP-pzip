#include "../../include/result_queue/init_result_queue.h"

// Function to initialize task queue
result_queue_t* init_result_queue(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize result queue: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for result queue structure and check for errors
    result_queue_t* result_queue = calloc(1,sizeof(result_queue_t)); 
    if (!result_queue) {
        
        // Failed to allocate memory for result queue structure
        fprintf(stderr, "Failed to initialize result queue: failed to allocate memory for result queue\n");
        return NULL; 
    }    

    // Initialize result_queue fields
    result_queue->front = 0;
    result_queue->rear = 0;
    result_queue->size = 0;
    result_queue->capacity = capacity; 
    result_queue->results = NULL; 
    result_queue->room_available = NULL; 
    result_queue->result_available = NULL; 
    result_queue->lock = NULL; 
    
    // Allocate memory for result
    if (!(result_queue->results = calloc(capacity, sizeof(encoding_result_t*)))) {
        
        // Failed to allocate memory for result
        fprintf(stderr, "Failed to initialize result queue: failed to allocate memory for results\n");
        free_result_queue(result_queue);
        return NULL; 
    }
    
    // Allocate memory for status_flags
    if (!(result_queue->status_flags = calloc(capacity, sizeof(int)))) {
        
        // Failed to allocate memory for result
        fprintf(stderr, "Failed to initialize result queue: failed to allocate memory for status flags\n");
        free_result_queue(result_queue);
        return NULL; 
    }
    for (size_t i = 0; i < capacity; i++) result_queue->status_flags[i] = EMPTY;
    
    // Allocate memory for mutex lock 
    if (!(result_queue->lock = calloc(1, sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for result_queue lock; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize result queue: failed to allocate memory for mutex lock\n");
        free_result_queue(result_queue);
        return NULL; 
        
    }
    
    // Initialize mutex lock 
    if (pthread_mutex_init(result_queue->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock
        fprintf(stderr, "Failed to initialize result queue: failed to initialize mutex lock\n");
        free_result_queue(result_queue);
        return NULL; 
    };
    
    // Allocate memory for condition variable
    if (!(result_queue->room_available = calloc(1, sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize result queue: failed to allocate memory for condition variable\n");
        free_result_queue(result_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(result_queue->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize result queue: failed to initialize condition variable\n");
        free_result_queue(result_queue);
        return NULL;
    }
    
    // Allocate memory for condition variable
    if (!(result_queue->result_available = calloc(1, sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize result queue: failed to allocate memory for condition variable\n");
        free_result_queue(result_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(result_queue->result_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize result queue: failed to initialize condition variable\n");
        free_result_queue(result_queue);
        return NULL;
    }
    
    // Successfully created result queue
    return result_queue; 
}

// EOF