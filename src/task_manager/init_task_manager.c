#include "../../include/task_manager/init_task_manager.h"

// Function to initialize a task manager with a given capacity
task_manager_t* init_task_manager(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize task manager: invalid input\n");
        return NULL;
    }
    
    // Initialize task manager and check for errors
    task_manager_t* manager = (task_manager_t*)malloc(sizeof(task_manager_t)); 
    if (!manager) {
        
        // Error; failed to allocate memory for task manager
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for task manager\n");
        return NULL; 
    }    

    // Set manager fields
    manager->tasks_front = 0;
    manager->results_front = 0; 
    manager->rear = 0;
    manager->size = 0;
    manager->num_available_tasks = 0; 
    manager->capacity = capacity; 
    
    // Allocate memory for an array of tasks_queue 
    if (!(manager->tasks_queue = (task_t**)malloc(sizeof(task_t*) * capacity))) {
        
        // Error; failed to allocate memory for tasks_queue array
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for tasks queue\n");
        return free_task_manager(manager);
    }
    
    // Allocate memory for an array of results_queue 
    if (!(manager->results_queue = (result_t**)malloc(sizeof(result_t*) * capacity))) {
        
        // Error; failed to allocate memory for tasks_queue array
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for results queue\n");
        return free_task_manager(manager);
    }
    
    // Initialize mutex lock for tasks_queue and check for errors
    if (!(manager->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for tasks_queue lock; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for mutex lock\n");
        return free_task_manager(manager);
        
    } else if (pthread_mutex_init(manager->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock for tasks_queue; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize task manager: failed to initialize mutex lock\n");
        return free_task_manager(manager);
    };
    
    // Initialize condition variable for signaling room is available and check for errors
    if (!(manager->room_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for room available condition variable; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for condition variable\n");
        return free_task_manager(manager);
        
    } else if (pthread_cond_init(manager->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize task manager: failed to initialize condition variable\n");
        return free_task_manager(manager);
    }
    
    // Initialize condition variable for signaling tasks_queue are available and check for errors
    if (!(manager->tasks_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for tasks_queue available condition variable; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for condition variable\n");
        return free_task_manager(manager);
        
    } else if (pthread_cond_init(manager->tasks_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize task manager: failed to initialize condition variable\n");
        return free_task_manager(manager);
    }
    
    // Initialize condition variable for signaling that results_queue are available and check for errors
    if (!(manager->next_result_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for results_queue available condition variable; perform cleanup routine and return
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for condition variable\n");
        return free_task_manager(manager);
        
    } else if (pthread_cond_init(manager->next_result_available, NULL) != 0) { 
            
        // Failed to initialize condition variable for results_queue available; free allocated memory and destroy mutex and condition variable
        fprintf(stderr, "Failed to initialize task manager: failed to initialize condition variable\n");
        return free_task_manager(manager);
    }
    
    // Initialize condition variable for signaling that tasks_queue are completed and check for errors
    if (!(manager->tasks_completed = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for tasks_queue completed condition variable
        fprintf(stderr, "Failed to initialize task manager: failed to allocate memory for condition variablen");
        return free_task_manager(manager);
        
    } else if (pthread_cond_init(manager->tasks_completed, NULL) != 0) { 
            
        // Failed to initialize tasks_queue completed condition variable
        fprintf(stderr, "Failed to initialize task manager: failed to initialize condition variable\n");
        return free_task_manager(manager);
    }
    
    // Successfully created manager
    return manager; 
}
