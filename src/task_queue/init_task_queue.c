#include "../../include/task_queue/init_task_queue.h"

// Function to initialize task queue
task_queue_t* init_task_queue(size_t capacity, result_queue_t* result_queue) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize tasks queue: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for tasks queue structure and check for errors
    task_queue_t* task_queue = calloc(1, sizeof(task_queue_t)); 
    if (!task_queue) {
        
        // Failed to allocate memory for tasks queue structure
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for tasks queue\n");
        return NULL; 
    }    

    // Initialize task_queue fields
    task_queue->head = 0;
    task_queue->tail = 0;
    task_queue->size = 0;
    task_queue->capacity = capacity; 
    task_queue->result_queue = result_queue; 
    
    // Allocate memory for tasks
    if (!(task_queue->tasks = calloc(capacity, sizeof(task_t*)))) {
        
        // Failed to allocate memory for tasks
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for tasks\n");
        free_task_queue(task_queue);
        return NULL; 
    }
    for (size_t i = 0; i < capacity; i++) task_queue->tasks[i] = NULL;
    
    // Allocate memory for mutex lock 
    if (!(task_queue->lock = calloc(1, sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for task_queue lock; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for mutex lock\n");
        free_task_queue(task_queue);
        return NULL; 
        
    }
    
    // Initialize mutex lock 
    if (pthread_mutex_init(task_queue->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock
        fprintf(stderr, "Failed to initialize tasks queue: failed to initialize mutex lock\n");
        free_task_queue(task_queue);
        return NULL; 
    };
    
    // Allocate memory for condition variable
    if (!(task_queue->room_available = calloc(1, sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for condition variable\n");
        free_task_queue(task_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(task_queue->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize tasks queue: failed to initialize condition variable\n");
        free_task_queue(task_queue);
        return NULL;
    }
    
    // Allocate memory for condition variable
    if (!(task_queue->tasks_available = calloc(1, sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for condition variable\n");
        free_task_queue(task_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(task_queue->tasks_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize tasks queue: failed to initialize condition variable\n");
        free_task_queue(task_queue);
        return NULL;
    }
    
    // Successfully created tasks queue
    return task_queue; 
}

// EOF