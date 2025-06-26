#include "../../include/task_manager/task_manager.h"
#include <pthread.h>

// Function to initialize a task manager with a given capacity
task_manager_t* init_manager(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Error: Capacity must be greater than zero\n");
        return NULL;
    }
    
    // Initialize task manager and check for errors
    task_manager_t* manager = (task_manager_t*)malloc(sizeof(task_manager_t)); 
    if (!manager) {
        
        // Error; failed to allocate memory for task manager
        fprintf(stderr, "Error: Failed to allocate memory for task manager\n");
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
        fprintf(stderr, "Error: Failed to allocate memory for task manager tasks_queue array\n");
        free_manager(manager);
        return NULL;
    }
    
    // Allocate memory for an array of results_queue 
    if (!(manager->results_queue = (result_t**)malloc(sizeof(result_t*) * capacity))) {
        
        // Error; failed to allocate memory for tasks_queue array
        fprintf(stderr, "Error: Failed to allocate memory for task manager results_queue array\n");
        free_manager(manager);
        return NULL;
    }
    
    // Initialize mutex lock for tasks_queue and check for errors
    if (!(manager->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for tasks_queue lock; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for task manager tasks_queue lock\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_mutex_init(manager->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock for tasks_queue; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to initialize mutex lock for tasks_queue\n");
        free_manager(manager);
        return NULL; 
    };
    
    // Initialize condition variable for signaling room is available and check for errors
    if (!(manager->room_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for room available condition variable; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for room available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Error: Failed to initialize condition variable for room available\n");
        free_manager(manager);
        return NULL; 
    }
    
    // Initialize condition variable for signaling tasks_queue are available and check for errors
    if (!(manager->tasks_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for tasks_queue available condition variable; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for tasks_queue available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->tasks_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Error: Failed to initialize condition variable for tasks_queue available\n");
        free_manager(manager);
        return NULL; 
    }
    
    // Initialize condition variable for signaling that results_queue are available and check for errors
    if (!(manager->next_result_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for results_queue available condition variable; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for results_queue available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->next_result_available, NULL) != 0) { 
            
        // Failed to initialize condition variable for results_queue available; free allocated memory and destroy mutex and condition variable
        fprintf(stderr, "Error: Failed to initialize condition variable for results_queue available\n");
        free_manager(manager);
        return NULL; 
    }
    
    // Initialize condition variable for signaling that tasks_queue are completed and check for errors
    if (!(manager->tasks_completed = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for tasks_queue completed condition variable
        fprintf(stderr, "Error: Failed to allocate memory for results_queue available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->tasks_completed, NULL) != 0) { 
            
        // Failed to initialize tasks_queue completed condition variable
        fprintf(stderr, "Error: Failed to initialize condition variable for results_queue available\n");
        free_manager(manager);
        return NULL; 
    }
    
    // Successfully created manager
    return manager; 
}

// Function to yield a termination task to the manager
int send_termination_signal(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {  // NULL tasks_queue are allowed as they act as termination signals
        
        // Invalid input
        fprintf(stderr, "Error: Manager is NULL\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to send termination signal: failed to acquire lock\n");
        return ERROR;
    }
        
    // Check if the manager is full
    while (manager->size == manager->capacity) {
        
        // Wait until there is space in the manager
        if (pthread_cond_wait(manager->room_available, manager->lock) != 0) {
            
            // Failed to wait for condition variable
            fprintf(stderr, "Failed to send termination signal: failed to wait for room available\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR;
        }
    }
            
    // Add termination task to the manager's task list
    manager->tasks_queue[manager->rear] = NULL;
    
    // Initiate a termination result
    if (!(manager->results_queue[manager->rear] = init_result())) {
        
        // Failed to initialize result
        fprintf(stderr, "Failed to send termination signal: failed to initialize result\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR;
    }
    
    // Set termination result fields
    manager->results_queue[manager->rear]->status = COMPLETED;
    manager->results_queue[manager->rear]->data = NULL; 
    
    // Determine if the result is the next result
    int send_results_available_signal = FALSE; 
    if (manager->rear == manager->results_front) send_results_available_signal = TRUE; 
        
    // Increment size, number of available tasks_queue and results_queue, and update rear index
    manager->size++;
    manager->num_available_tasks++; 
    manager->rear = (manager->rear + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR; 
    }
    
    // Check if the result is the next result 
    if (send_results_available_signal) {
            
        // Signal to the writer that a result is available
        if (pthread_cond_signal(manager->next_result_available) != SUCCESS) {
            
            // Failed to signal condition variable
            fprintf(stderr, "Error: Failed to signal condition variable\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR; 
        }
    }
            
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR;
    }
    
    // Successfully added tasks_queue
    return SUCCESS;
}

// Function to free the task manager
void* free_manager(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to free manager: Invalid input\n");
        return NULL; 
    }
    
    // Check if mutex lock exists
    if (manager->lock) {
        
        // Destroy mutex lock for tasks_queue
        pthread_mutex_destroy(manager->lock);
        free(manager->lock);
    }
    
    // Check if condition variable for room availability exists
    if (manager->room_available) {
        
        // Destroy condition variable for room availability
        pthread_cond_destroy(manager->room_available);
        free(manager->room_available);
    }   
    
    // Check if condition variable for tasks_queue availability exists
    if (manager->tasks_available) {
        
        // Destroy condition variable for tasks_queue availability
        pthread_cond_destroy(manager->tasks_available);
        free(manager->tasks_available);
    }
    
    // Check if condition variable for results_queue availability exists
    if (manager->next_result_available) {
        
        // Destroy condition variable for results_queue availability
        pthread_cond_destroy(manager->next_result_available);
        free(manager->next_result_available);
    }
    
    // Check if condition variable for taskscompletion exists
    if (manager->tasks_completed) {
        
        // Destroy condition variable for completion
        pthread_cond_destroy(manager->tasks_completed);
        free(manager->tasks_completed);
    }
    
    // Free memory allocated for tasks_queue and results_queue arrays
    if (manager->tasks_queue) free(manager->tasks_queue);
    if (manager->results_queue) free(manager->results_queue);
    
    // Free memory allocated for manager structure
    free(manager); 
    
    // Successfully freed manager
    return NULL; 
}

// Function to wait for all tasks_queue to complete
int wait_for_completion(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to wait for completion: Invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if(pthread_mutex_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to wait for completion: failed to acquire lock\n");
        return ERROR;
    }
    
    for (int i = 0; i < manager->capacity; i++) {
        if (manager->results_queue[i]) {
            if (manager->results_queue[i]->status == COMPLETED && manager->results_queue[i]->data == NULL) {
                fprintf(stderr, "Result %d is TERMINATION\n", i); 
            } else if (manager->results_queue[i]->status == COMPLETED && manager->results_queue[i]->data != NULL) {
                fprintf(stderr, "Result %d is COMPLETED\n", i); 
            }
        }
    }
    
    // Wait for all tasks_queue to complete
    while (manager->size > 0) {
        fprintf(stderr, "Waiting for tasks_queue to complete (%zu tasks_queue left)...\n", manager->size);
        
        // Wait for tasks_queue to become available
        if (pthread_cond_wait(manager->tasks_completed, manager->lock) != SUCCESS) {
            
            // Failed to wait for condition variable
            fprintf(stderr, "Failed to wait for completion: error occurred while waiting for tasks_queue\n");
            pthread_mutex_unlock(manager->lock);
            return ERROR;
        }
    }
    
    // Release lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to wait for completion: failed to release lock\n");
        pthread_mutex_unlock(manager->lock);
        return ERROR;
    }
    
    // Successfully waited for completion
    return SUCCESS;
}