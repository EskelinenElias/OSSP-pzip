#include "../include/task_manager.h"

int acquire_lock(pthread_mutex_t* lock) {
    return pthread_mutex_lock(lock);
}

// Function to initialize a task manager with a given capacity
task_manager_t* init_manager(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Error: Capacity must be greater than zero\n");
        return NULL;
    }
    
    // Initialize task manager and check for errors
    task_manager_t* manager = malloc(sizeof(task_manager_t)); 
    if (!manager) {
        
        // Error; failed to allocate memory for task manager
        fprintf(stderr, "Error: Failed to allocate memory for task manager\n");
        return NULL; 
    }    

    // Set manager fields
    manager->next_task = 0;
    manager->next_result = 0; 
    manager->next_slot = 0, 
    manager->current_size = 0; 
    manager->capacity = capacity; 
    
    // Allocate memory for tasks array
    manager->tasks = malloc(sizeof(task_t*) * capacity);
    if (!manager->tasks) {
        
        // Error; failed to allocate memory for tasks array
        fprintf(stderr, "Error: Failed to allocate memory for task manager tasks array\n");
        free_manager(manager);
        return NULL;
    }
    
    // Allocate memory for results array
    manager->results = malloc(sizeof(result_t*) * capacity);
    if (!manager->results) {
        
        // Error; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for task manager results array\n");
        free_manager(manager);
        return NULL;
    }

    // Initialize mutex lock for tasks and check for errors
    manager->tasks_lock = malloc(sizeof(pthread_mutex_t));
    if (!manager->tasks_lock) {
        
        // Failed to allocate memory for tasks lock; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for task manager tasks lock\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_mutex_init(manager->tasks_lock, NULL) != 0) {
        
        // Failed to initialize mutex lock for tasks; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to initialize mutex lock for tasks\n");
        free_manager(manager);
        return NULL; 
    };
    
    // // Initialize mutex lock for results and check for errors
    // manager->results_lock = malloc(sizeof(pthread_mutex_t));
    // if (!manager->results_lock) {
        
    //     // Failed to allocate memory for results lock; perform cleanup routine and return
    //     fprintf(stderr, "Error: Failed to allocate memory for task manager results lock\n");
    //     free_manager(manager);
    //     return NULL;
        
    // } else if (pthread_mutex_init(manager->results_lock, NULL) != 0) {
        
    //     // Failed to initialize mutex lock for results; perform cleanup routine and return
    //     fprintf(stderr, "Error: Failed to initialize mutex lock for results\n");
    //     free_manager(manager);
    //     return NULL; 
    // };
    
    // Initialize condition variable for signaling room is available and check for errors
    manager->room_available = malloc(sizeof(pthread_cond_t));
    if (!manager->room_available) {
        
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
    
    // Initialize condition variable for signaling tasks are available and check for errors
    manager->tasks_available = malloc(sizeof(pthread_cond_t));
    if (!manager->tasks_available) {
        
        // Failed to allocate memory for tasks available condition variable; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for tasks available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->tasks_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Error: Failed to initialize condition variable for tasks available\n");
        free_manager(manager);
        return NULL; 
    }
    
    // Initialize condition variable for signaling that results are available and check for errors
    manager->results_available = malloc(sizeof(pthread_cond_t));
    if (!manager->results_available) {
        
        // Failed to allocate memory for results available condition variable; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for results available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->results_available, NULL) != 0) { 
            
        // Failed to initialize condition variable for results available; free allocated memory and destroy mutex and condition variable
        fprintf(stderr, "Error: Failed to initialize condition variable for results available\n");
        free_manager(manager);
        return NULL; 
    }
    
    // Successfully created manager
    return manager; 
}

// Function for yielding a task to the manager (by main thread)
int yield_task(task_manager_t* manager, char* data, size_t length) {
    
    // Input validation
    if (!manager) {  // NULL tasks are allowed as they act as termination signals
        
        // Invalid input
        fprintf(stderr, "Error: Manager is NULL\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(manager->tasks_lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if the manager is full
    while (manager->current_size == manager->capacity) {
        pthread_cond_wait(manager->room_available, manager->tasks_lock);
    }
        
    // Create a new task and check for errors
    task_t* task = create_task(data, length, manager->next_slot);
    if (!task) {
        
        // Failed to create task
        fprintf(stderr, "Error: Failed to create task\n");
        return ERROR; 
    }
    
    // Add task to the manager's task list
    manager->tasks[manager->next_slot] = task;
    
    // Increment size and update rear index
    manager->current_size++;
    manager->next_slot = (manager->next_slot + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != 0) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(manager->tasks_lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return ERROR;
    }
    
    // Successfully added tasks
    return SUCCESS;
}

// Function to claim a task from the manager
task_t* claim_task(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Error: Manager is NULL\n");
        return NULL; 
    }
    
    // Acquire lock
    if (pthread_mutex_lock(manager->tasks_lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return NULL;
    }
    
    // Check if tasks are available 
    while (!manager->tasks[manager->next_task]) {
        
        // Wait for tasks to become available
        pthread_cond_wait(manager->tasks_available, manager->tasks_lock);
    }
    
    // Claim a task from the manager and check if it's valid
    task_t* task = manager->tasks[manager->next_task]; // NULL pointer represents a termination signal
    
    // Increment next task index
    manager->next_task = (manager->next_task + 1) % manager->capacity;
    
    // Release the lock
    if (pthread_mutex_unlock(manager->tasks_lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return NULL;
    }
    
    // Return claimed task
    return task;
}

// Function to yield the result of a task to the manager (by a worker)
int yield_result(task_manager_t* manager, result_t* result, size_t task_index) {
    
    // Input validation
    if (!manager || !result || task_index < 0 || task_index >= manager->capacity) {
        
        // Invalid input
        fprintf(stderr, "Error: Invalid input\n");
        return ERROR; 
    } 
    
    // Acquire lock
    if (pthread_mutex_lock(manager->tasks_lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Add the result to the managers results array
    manager->results[task_index] = result;
    
    // Free the memory allocated for the task 
    if (free_task(manager->tasks[task_index]) != SUCCESS) return ERROR;
    
    // Set the task pointer to NULL
    manager->tasks[task_index] = NULL; 
    
    // Signal main thread that results are available
    if (pthread_cond_signal(manager->results_available) != 0) {
        
        // Failed to signal main thread that results are available
        fprintf(stderr, "Error: Failed to signal that results are available\n");
        return ERROR;
    }
    
    // Release the lock
    if (pthread_mutex_unlock(manager->tasks_lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return NULL;
    }
    
    // Successfully yielded result
    return SUCCESS;
}

// Function to claim a result from the task manager (in order)
result_t* claim_result(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Error: Invalid input\n");
        return NULL;
    }
    
    // Acquire lock
    if (pthread_mutex_lock(manager->tasks_lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return NULL;
    }
    
    // Check if results are available
    while (!manager->results[manager->next_result]) {
        
        // Wait for results to become available
        pthread_cond_wait(manager->results_available, manager->tasks_lock);
    }
    
    // Claim a result from the manager and check that it's valid
    result_t* result = manager->results[manager->next_result];
    if (!result) {
        
        // Invalid result
        fprintf(stderr, "Error: Invalid result\n");
        pthread_mutex_unlock(manager->tasks_lock);
        return NULL;
    }
    
    // Set the result array slot to NULL 
    manager->results[manager->next_result] = NULL;
    
    // Increment next result index and decrement current size
    manager->next_result = (manager->next_result + 1) % manager->capacity;
    manager->current_size--; 
    
    // Release the lock
    if (pthread_mutex_unlock(manager->tasks_lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return NULL;
    }
    
    // Return claimed result
    return result;
}

// Function to yield a termination task to the manager
int yield_termination_task(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {  // NULL tasks are allowed as they act as termination signals
        
        // Invalid input
        fprintf(stderr, "Error: Manager is NULL\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(manager->tasks_lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if the manager is full
    while (manager->current_size == manager->capacity) {
        pthread_cond_wait(manager->room_available, manager->tasks_lock);
    }
            
    // Add termination task to the manager's task list
    manager->tasks[manager->next_slot] = NULL;
    
    // Increment size and update rear index
    manager->current_size++;
    manager->next_slot = (manager->next_slot + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != 0) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(manager->tasks_lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return ERROR;
    }
    
    // Successfully added tasks
    return SUCCESS;
}


// Function to free the task manager
void free_manager(task_manager_t* manager) {
    
    // Input validation
    if (!manager) return; 
    
    // Destroy mutex lock for tasks
    if (manager->tasks_lock) {
        pthread_mutex_destroy(manager->tasks_lock);
        free(manager->tasks_lock);
    }
    
    // Destroy condition variable for room availability
    if (manager->room_available) {
        pthread_cond_destroy(manager->room_available);
        free(manager->room_available);
    }   
    
    // Destroy condition variable for tasks availability
    if (manager->tasks_available) {
        pthread_cond_destroy(manager->tasks_available);
        free(manager->tasks_available);
    }
    
    // Destroy condition variable for results availability
    if (manager->results_available) {
        pthread_cond_destroy(manager->results_available);
        free(manager->results_available);
    }
    
    // Free memory
    if (manager->tasks) free(manager->tasks);
    if (manager->results) free(manager->results);
    free(manager); 
}