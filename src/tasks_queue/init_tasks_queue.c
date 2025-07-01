#include "../../include/tasks_queue/init_tasks_queue.h"

// Function to initialize task queue
tasks_queue_t* init_tasks_queue(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize tasks queue: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for tasks queue structure and check for errors
    tasks_queue_t* tasks_queue = (tasks_queue_t*)malloc(sizeof(tasks_queue_t)); 
    if (!tasks_queue) {
        
        // Failed to allocate memory for tasks queue structure
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for tasks queue\n");
        return NULL; 
    }    

    // Initialize tasks_queue fields
    tasks_queue->front = 0;
    tasks_queue->rear = 0;
    tasks_queue->size = 0;
    tasks_queue->capacity = capacity; 
    tasks_queue->tasks = NULL; 
    tasks_queue->room_available = NULL; 
    tasks_queue->tasks_available = NULL; 
    tasks_queue->lock = NULL; 
    
    // Allocate memory for tasks
    if (!(tasks_queue->tasks = (task_data_t**)malloc(sizeof(task_data_t*) * capacity))) {
        
        // Failed to allocate memory for tasks
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for tasks\n");
        free_tasks_queue(tasks_queue);
        return NULL; 
    }
    
    // Allocate memory for mutex lock 
    if (!(tasks_queue->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for tasks_queue lock; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for mutex lock\n");
        free_tasks_queue(tasks_queue);
        return NULL; 
        
    }
    
    // Initialize mutex lock 
    if (pthread_mutex_init(tasks_queue->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock
        fprintf(stderr, "Failed to initialize tasks queue: failed to initialize mutex lock\n");
        free_tasks_queue(tasks_queue);
        return NULL; 
    };
    
    // Allocate memory for condition variable
    if (!(tasks_queue->room_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for condition variable\n");
        free_tasks_queue(tasks_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(tasks_queue->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize tasks queue: failed to initialize condition variable\n");
        free_tasks_queue(tasks_queue);
        return NULL;
    }
    
    // Allocate memory for condition variable
    if (!(tasks_queue->tasks_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for condition variable
        fprintf(stderr, "Failed to initialize tasks queue: failed to allocate memory for condition variable\n");
        free_tasks_queue(tasks_queue);
        return NULL; 
        
    } 
   
    // Initialize condition variable
    if (pthread_cond_init(tasks_queue->tasks_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize tasks queue: failed to initialize condition variable\n");
        free_tasks_queue(tasks_queue);
        return NULL;
    }
    
    // Successfully created tasks queue
    return tasks_queue; 
}

// EOF