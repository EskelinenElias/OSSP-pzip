#include "../include/results_manager.h"

// Function to initialize a task manager with a given capacity
results_manager_t* init_results_manager(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize results manager: invalid input\n");
        return NULL;
    }
    
    // Initialize task manager and check for errors
    results_manager_t* manager = (results_manager_t*)malloc(sizeof(results_manager_t)); 
    if (!manager) {
        
        // Error; failed to allocate memory for task manager
        fprintf(stderr, "Failed to initialize results manager: failed to allocate memory for results manager\n");
        return NULL; 
    }    

    // Set manager fields
    manager->front = 0;
    manager->rear = 0;
    manager->size = 0;
    manager->capacity = capacity; 
    
    // Allocate memory for an array of tasks 
    if (!(manager->results_queue = (result_t**)malloc(sizeof(task_t*) * capacity))) {
        
        // Error; failed to allocate memory for tasks array
        fprintf(stderr, "Failed to initialize results manager: failed to allocate memory for results\n");
        free_results_manager(manager);
        return NULL;
    }
    
    // Initialize mutex lock for tasks and check for errors
    if (!(manager->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for mutex lock
        fprintf(stderr, "Failed to initialize results manager: failed to allocate memory for mutex lock\n");
        free_results_manager(manager);
        return NULL;
        
    } else if (pthread_mutex_init(manager->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock
        fprintf(stderr, "Failed to initialize results manager: failed to initialize mutex lock\n");
        free_results_manager(manager);
        return NULL; 
    };
    
    // Initialize condition variable for signaling room is available and check for errors
    if (!(manager->room_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for room available condition variable
        fprintf(stderr, "Failed to initialize results manager: failed to allocate memory for room available condition variable\n");
        free_results_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->room_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        fprintf(stderr, "Failed to initialize results manager: failed to initialize room available condition variable\n");
        free_results_manager(manager);
        return NULL; 
    }
    
    // Initialize condition variable for signaling that the next result is available and check for errors
    if (!(manager->next_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for results available condition variable
        fprintf(stderr, "Failed to initialize results manager: failed to allocate memory for next available condition variable\n");
        free_results_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->next_available, NULL) != 0) { 
            
        // Failed to initialize condition variable for results available
        fprintf(stderr, "Failed to initialize results manager: failed to initialize next available condition variable\n");
        free_results_manager(manager);
        return NULL; 
    }
    
    // Successfully created manager
    return manager; 
}

// Function to acquire a lock 
int acquire_lock(pthread_mutex_t* lock) {
    
    // Input validation
    if (!lock) {
        
        // Invalid input
        fprintf(stderr, "Failed to acquire lock: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield task: failed to acquire lock\n");
        return ERROR;
    }
    
    // Successfully acquired lock
    return SUCCESS;
}

// Function to release a lock 
int release_lock(pthread_mutex_t* lock) {
    
    // Input validation
    if (!lock) {
        
        // Invalid input
        fprintf(stderr, "Failed to release lock: invalid input\n");
        return ERROR;
    }
    
    // Release lock 
    if (pthread_mutex_unlock(lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield task: failed to release lock\n");
        return ERROR;
    }
    
    // Successfully released lock
    return SUCCESS;
}

// Function to send a signal 
int send_signal(pthread_cond_t* condition) {
    
    // Input validation
    if (!condition) {
        
        // Invalid input
        fprintf(stderr, "Failed to send signal: invalid input\n");
        return ERROR;
    }
    
    // Send signal 
    if (pthread_cond_signal(condition) != SUCCESS) {
        
        // Failed to send signal
        fprintf(stderr, "Failed to send signal: failed to send signal\n");
        return ERROR;
    }
    
    // Successfully sent signal
    return SUCCESS;
}

// Function to wait for signal 
int wait_for_signal(pthread_cond_t* condition, pthread_mutex_t* lock) {
    
    // Input validation
    if (!condition || !lock) {
        
        // Invalid input
        fprintf(stderr, "Failed to wait for signal: invalid input\n");
        return ERROR;
    }
    
    // Wait for signal 
    if (pthread_cond_wait(condition, lock) != SUCCESS) {
        
        // Failed to wait for signal 
        fprintf(stderr, "Failed to wait for signal: failed to wait for signal\n");
        return ERROR;
    }
    
    // Successfully waited for signal 
    return SUCCESS;
}

// Function for yielding a task to the manager (by main thread)
int yield_task(task_manager_t* manager, char* data, size_t size) {
    
    // Input validation
    if (!manager) { // Empty tasks (task_data = NULL, task_size = 0) are allowed, as they act as EOF
        
        // Invalid input
        fprintf(stderr, "Failed to yield task: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (acquire_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield task: failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if the manager is full and wait for room to become available
    while (manager->size == manager->capacity) {
        
        // Wait for signal that room is available
        if (wait_for_signal(manager->room_available, manager->lock) != SUCCESS) {
            
            // Failed to wait for signal 
            fprintf(stderr, "Failed to yield task: failed to wait for room\n");
            release_lock(manager->lock); 
            return ERROR;
        }
    }
    
    // Initialize the result corresponding to the task
    result_t* result = init_result();
    if (!result) {
        
        // Failed to initialize result
        fprintf(stderr, "Failed to yield task: failed to initialize result\n");
        release_lock(manager->lock);
        return ERROR;
    }
    
    // Initialize a new task and check for errors
    task_t* task = init_task(data, size, result);
    if (!task) {
        
        // Failed to initialize task
        fprintf(stderr, "Failed to yield task: failed to initialize task\n");
        release_lock(manager->lock);
        return ERROR;
    }
    
    // Add the task and result to the manager
    manager->tasks[manager->rear] = task;
    manager->results[manager->rear] = result;
    
    // Increment number of available tasks and size and update rear index
    manager->num_available_tasks++; 
    manager->size++;
    manager->rear = (manager->rear + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (send_signal(manager->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Failed to yield task: failed to signal that a task is available\n");
        release_lock(manager->lock);
        return ERROR; 
    }
        
    // Release the lock
    if (release_lock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield task: failed to release lock\n");
        release_lock(manager->lock); 
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
        fprintf(stderr, "Failed to claim task: invalid input\n");
        return NULL; 
    }
    
    // Acquire lock
    if (acquire_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim task: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if tasks are available 
    while (manager->num_available_tasks == 0) {
        
        // Wait for tasks to become available
        if (wait_for_signal(manager->tasks_available, manager->lock) != 0) {
            
            // Failed to wait for tasks to become available
            fprintf(stderr, "Failed to claim task: failed to wait for tasks to become available\n");
            release_lock(manager->lock);
            return NULL;
        }
    }
    
    // Claim a task from the manager and check if it's valid
    task_t* task = manager->tasks[manager->next_task_index]; // NULL pointer represents a termination signal
    manager->tasks[manager->next_task_index] = NULL; // Set the tasks array slot to NULL (to avoid dangling pointers)
    
    // Increment next task index and decrement number of available tasks
    manager->next_task_index = (manager->next_task_index + 1) % manager->capacity;
    manager->num_available_tasks--;
        
    // Release the lock
    if (release_lock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim task: failed to release lock\n");
        release_lock(manager->lock); 
        return NULL;
    }
        
    // Return claimed task
    return task;
}

// Function to yield the result of a task to the manager (by a worker)
int yield_result(task_manager_t* manager, result_t* result, result_data_t* result_data) {
    
    // Input validation
    if (!manager || !result) {
        
        // Invalid input
        fprintf(stderr, "Failed to yield result: invalid input\n");
        return ERROR; 
    } 
    
    // Acquire lock
    if (acquire_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to yield result: failed to acquire lock\n");
        return ERROR;
    }
    
    // Add the result to the managers results array
    result->data = result_data;
    result->status = COMPLETED; // Set the status flag to COMPLETED
    
    // Check if the result is the next result in the queue
    if (manager->results[manager->next_result_index] == result) {
    
        // Signal that the next result is available
        if (pthread_cond_signal(manager->next_result_available) != SUCCESS) {
            
            // Failed to signal main thread that results are available
            fprintf(stderr, "Failed to yield result: failed to signal that results are available\n");
            release_lock(manager->lock);
            return ERROR;
        }
    }
    
    // Release the lock
    if (release_lock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to yield result: failed to release lock\n");
        release_lock(manager->lock); 
        return ERROR;
    }
    
    // Successfully yielded result
    return SUCCESS;
}

// Function to claim a result from the task manager (in order)
result_data_t* claim_result(task_manager_t* manager) {
        
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to claim result: invalid input\n");
        return NULL;
    }
        
    // Acquire lock
    if (acquire_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to claim result: failed to acquire lock\n");
        return NULL;
    }
    
    // Check if the next result is available
    while (manager->results[manager->next_result_index] == NULL || manager->results[manager->next_result_index]->status != COMPLETED) {
                
        // Wait for the next result to become available
        if (wait_for_signal(manager->next_result_available, manager->lock) != SUCCESS) {
            
            // Failed to wait for result
            fprintf(stderr, "Failed to claim result: failed to wait for result\n");
            release_lock(manager->lock);
            return NULL;
        }
    }
    
    // Get result data and free the result
    result_data_t* data = manager->results[manager->next_result_index]->data;
    manager->results[manager->next_result_index]->data = NULL; // Set the result data to NULL (to avoid dangling pointers)
    
    // Free the result and set the result array pointer to NULL (to avoid dangling pointers)
    free_result(manager->results[manager->next_result_index]);
    manager->results[manager->next_result_index] = NULL; 

    // Increment next result index and decrement current size
    manager->next_result_index = (manager->next_result_index + 1) % manager->capacity;
    manager->size--; 
    
    // Signal that the task manager has room available
    if (pthread_cond_signal(manager->room_available) != 0) {
        
        // Failed to signal room available
        fprintf(stderr, "Failed to claim result: failed to signal that room is available\n");
        release_lock(manager->lock);
        return NULL;
    }
    
    // Check if all tasks have been completed
    if (manager->size == 0) {
        
        // All tasks completed: Signal that all tasks have been completed
        if (send_signal(manager->tasks_completed) != 0) {
            
            // Failed to signal all tasks completed
            fprintf(stderr, "Failed to claim result: failed to signal all tasks completed\n");
            release_lock(manager->lock);
            return NULL;
        }
    }
    
    // Release the lock
    if (release_lock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to claim result: failed to release lock\n");
        release_lock(manager->lock);
        return NULL;
    }
        
    // Return claimed result
    return data;
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
    if (acquire_lock(manager->lock) != SUCCESS) {
        
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
            release_lock(manager->lock);
            return ERROR;
        }
    }
            
    // Add termination task to the manager's task list
    manager->tasks[manager->rear] = NULL;
    
    // Initiate a termination result
    if (!(manager->results[manager->rear] = init_result())) {
        
        // Failed to initialize result
        fprintf(stderr, "Failed to send termination signal: failed to initialize result\n");
        release_lock(manager->lock);
        return ERROR;
    }
    
    // Set termination result fields
    manager->results[manager->rear]->status = COMPLETED;
    manager->results[manager->rear]->data = NULL; 
    
    // Determine if the result is the next result
    int send_results_available_signal = FALSE; 
    if (manager->rear == manager->next_result_index) send_results_available_signal = TRUE; 
        
    // Increment size, number of available tasks and results, and update rear index
    manager->size++;
    manager->num_available_tasks++; 
    manager->rear = (manager->rear + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (send_signal(manager->tasks_available) != SUCCESS) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        release_lock(manager->lock);
        return ERROR; 
    }
    
    // Check if the result is the next result 
    if (send_results_available_signal) {
            
        // Signal to the writer that a result is available
        if (send_signal(manager->next_result_available) != SUCCESS) {
            
            // Failed to signal condition variable
            fprintf(stderr, "Error: Failed to signal condition variable\n");
            release_lock(manager->lock);
            return ERROR; 
        }
    }
            
    // Release the lock
    if (release_lock(manager->lock) != SUCCESS) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        release_lock(manager->lock);
        return ERROR;
    }
    
    // Successfully added tasks
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
        
        // Destroy mutex lock for tasks
        pthread_mutex_destroy(manager->lock);
        free(manager->lock);
    }
    
    // Check if condition variable for room availability exists
    if (manager->room_available) {
        
        // Destroy condition variable for room availability
        pthread_cond_destroy(manager->room_available);
        free(manager->room_available);
    }   
    
    // Check if condition variable for tasks availability exists
    if (manager->tasks_available) {
        
        // Destroy condition variable for tasks availability
        pthread_cond_destroy(manager->tasks_available);
        free(manager->tasks_available);
    }
    
    // Check if condition variable for results availability exists
    if (manager->next_result_available) {
        
        // Destroy condition variable for results availability
        pthread_cond_destroy(manager->next_result_available);
        free(manager->next_result_available);
    }
    
    // Check if condition variable for taskscompletion exists
    if (manager->tasks_completed) {
        
        // Destroy condition variable for completion
        pthread_cond_destroy(manager->tasks_completed);
        free(manager->tasks_completed);
    }
    
    // Free memory allocated for tasks and results arrays
    if (manager->tasks) free(manager->tasks);
    if (manager->results) free(manager->results);
    
    // Free memory allocated for manager structure
    free(manager); 
    
    // Successfully freed manager
    return NULL; 
}

// Function to wait for all tasks to complete
int wait_for_completion(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to wait for completion: Invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if(acquire_lock(manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to wait for completion: failed to acquire lock\n");
        return ERROR;
    }
    
    for (int i = 0; i < manager->capacity; i++) {
        if (manager->results[i]) {
            if (manager->results[i]->status == COMPLETED && manager->results[i]->data == NULL) {
                fprintf(stderr, "Result %d is TERMINATION\n", i); 
            } else if (manager->results[i]->status == COMPLETED && manager->results[i]->data != NULL) {
                fprintf(stderr, "Result %d is COMPLETED\n", i); 
            }
        }
    }
    
    // Wait for all tasks to complete
    while (manager->size > 0) {
        fprintf(stderr, "Waiting for tasks to complete (%zu tasks left)...\n", manager->size);
        
        // Wait for tasks to become available
        if (wait_for_signal(manager->tasks_completed, manager->lock) != SUCCESS) {
            
            // Failed to wait for condition variable
            fprintf(stderr, "Failed to wait for completion: error occurred while waiting for tasks\n");
            release_lock(manager->lock);
            return ERROR;
        }
    }
    
    // Release lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Failed to wait for completion: failed to release lock\n");
        release_lock(manager->lock);
        return ERROR;
    }
    
    // Successfully waited for completion
    return SUCCESS;
}