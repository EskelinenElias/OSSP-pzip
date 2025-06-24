#include "../include/task_manager.h"

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
    manager->next_task_index = 0;
    manager->next_result_index = 0; 
    manager->front = 0;
    manager->rear = 0;
    manager->size = 0;
    manager->num_available_tasks = 0; 
    manager->num_available_results = 0;
    manager->capacity = capacity; 
    
    // Allocate memory for an array of tasks 
    manager->tasks = malloc(sizeof(task_t*) * capacity);
    if (!manager->tasks) {
        
        // Error; failed to allocate memory for tasks array
        fprintf(stderr, "Error: Failed to allocate memory for task manager tasks array\n");
        free_manager(manager);
        return NULL;
    }
    
    // Allocate memory for an array of results 
    manager->results = malloc(sizeof(result_t*) * capacity);
    if (!manager->results) {
        
        // Error; failed to allocate memory for tasks array
        fprintf(stderr, "Error: Failed to allocate memory for task manager results array\n");
        free_manager(manager);
        return NULL;
    }
    
    // Allocate memory for an array of statuses 
    manager->completed = malloc(sizeof(int) * capacity);
    if (!manager->completed) {
        
        // Error; failed to allocate memory for tasks array
        fprintf(stderr, "Error: Failed to allocate memory for task manager completed array\n");
        free_manager(manager);
        return NULL;
    }
    
    // Initialize mutex lock for tasks and check for errors
    if (!(manager->lock = malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for tasks lock; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to allocate memory for task manager tasks lock\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_mutex_init(manager->lock, NULL) != 0) {
        
        // Failed to initialize mutex lock for tasks; perform cleanup routine and return
        fprintf(stderr, "Error: Failed to initialize mutex lock for tasks\n");
        free_manager(manager);
        return NULL; 
    };
    
    // Initialize condition variable for signaling room is available and check for errors
    if (!(manager->room_available = malloc(sizeof(pthread_cond_t)))) {
        
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
    if (!(manager->tasks_available = malloc(sizeof(pthread_cond_t)))) {
        
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
    if (!(manager->results_available = malloc(sizeof(pthread_cond_t)))) {
        
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
    
    // Initialize condition variable for signaling that tasks are completed and check for errors
    if (!(manager->tasks_completed = malloc(sizeof(pthread_cond_t)))) {
        
        // Failed to allocate memory for tasks completed condition variable
        fprintf(stderr, "Error: Failed to allocate memory for results available condition variable\n");
        free_manager(manager);
        return NULL;
        
    } else if (pthread_cond_init(manager->tasks_completed, NULL) != 0) { 
            
        // Failed to initialize tasks completed condition variable
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
    if (!manager) { // Empty tasks (data = NULL, length = 0) are allowed, as they act as EOF
        
        // Invalid input
        fprintf(stderr, "Error: Manager is NULL\n");
        return ERROR;
    }
    
    // Acquire lock 
    if (pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if the manager is full and wait for room to become available
    while (manager->size == manager->capacity) {
        
        // Wait for signal that room is available
        if (pthread_cond_wait(manager->room_available, manager->lock) != 0) {
            
            // Failed to wait for signal 
            fprintf(stderr, "Error: Failed to wait for room available signal\n");
            return ERROR;
        }
    }
        
    // Create a new task and check for errors
    task_t* task = init_task(data, length, &manager->results[manager->rear], &manager->completed[manager->rear]);
    if (!task) {
        
        // Failed to initialize task
        fprintf(stderr, "Error: Failed to initialize task\n");
        return ERROR; 
    }
    
    // Add task to the manager's task list
    manager->tasks[manager->rear] = task;
    manager->completed[manager->rear] = FALSE; 
    manager->results[manager->rear] = NULL; 
    
    // Increment number of available tasks and size and update rear index
    manager->num_available_tasks++; 
    manager->size++;
    manager->rear = (manager->rear + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != 0) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
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
    if (pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return NULL;
    }
    
    // Check if tasks are available 
    while (manager->num_available_tasks == 0) {
        
        // Wait for tasks to become available
        pthread_cond_wait(manager->tasks_available, manager->lock);
    }
    
    // Claim a task from the manager and check if it's valid
    task_t* task = manager->tasks[manager->next_task_index]; // NULL pointer represents a termination signal
    
    // Set the tasks array slot to NULL (to avoid dangling pointers)
    manager->tasks[manager->next_task_index] = NULL;
    
    // Increment next task index and decrement number of available tasks
    manager->next_task_index = (manager->next_task_index + 1) % manager->capacity;
    manager->num_available_tasks--;
    
    // Check if the manager is empty
    if (manager->size == 0) {
        
        // Signal that all tasks have been completed
        if (pthread_cond_signal(manager->tasks_completed) != 0) {
            
            // Failed to signal tasks completed
            fprintf(stderr, "Error: Failed to signal tasks_completed\n");
            return NULL; 
        }
    }
        
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return NULL;
    }
        
    // Return claimed task
    return task;
}

// Function to yield the result of a task to the manager (by a worker)
int yield_result(task_manager_t* manager, task_t* task, result_t* result) {
    
    // Input validation
    if (!manager || !task || !result) {
        
        // Invalid input
        fprintf(stderr, "Error: Invalid input\n");
        return ERROR; 
    } 
    
    // Acquire lock
    if (pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Add the result to the managers results array
    *task->result = result;
    *task->status = TRUE; 
    manager->num_available_results++;
    
    // Free the memory allocated for the task 
    if (free_task(task) != SUCCESS) {
        
        // Failed to free task
        fprintf(stderr, "Error: Failed to free task\n");
        return ERROR;
    }
    
    // Signal main thread that results are available
    if (pthread_cond_signal(manager->results_available) != 0) {
        
        // Failed to signal main thread that results are available
        fprintf(stderr, "Error: Failed to signal that results are available\n");
        return ERROR;
    }
    
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return ERROR;
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
    if (pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return NULL;
    }
    
    // Check if the next result is available
    while (!(manager->completed[manager->next_result_index])) {
                
        // Wait for the next result to become available
        pthread_cond_wait(manager->results_available, manager->lock);
    }
            
    // Claim a result from the manager and check that it's valid
    result_t* result = manager->results[manager->next_result_index];
    
    // Set the result array slot to NULL 
    manager->results[manager->next_result_index] = NULL;
    
    // Increment next result index and decrement current size
    manager->next_result_index = (manager->next_result_index + 1) % manager->capacity;
    manager->num_available_results--; 
    manager->front = (manager->front + 1) % manager->capacity;
    manager->size--; 
    
    // Signal that the task manager has room available
    if (pthread_cond_signal(manager->room_available) != 0) {
        
        // Failed to signal room available
        fprintf(stderr, "Error: Failed to signal room available\n");
        return NULL;
    }
    
    // Check if all tasks have been completed
    if (manager->size == 0) {
        
        // All tasks completed: Signal that all tasks have been completed
        if (pthread_cond_signal(manager->tasks_completed) != 0) {
            
            // Failed to signal all tasks completed
            fprintf(stderr, "Error: Failed to signal all tasks completed\n");
            return NULL;
        }
    }
    
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
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
    if (pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Check if the manager is full
    while (manager->size == manager->capacity) {
        
         // Wait until there is space in the manager
         pthread_cond_wait(manager->room_available, manager->lock);
    }
            
    // Add termination task to the manager's task list
    manager->tasks[manager->rear] = NULL;
    manager->results[manager->rear] = NULL; 
    manager->completed[manager->rear] = TRUE; 
    
    // Increment size, number of available tasks and results, and update rear index
    manager->size++;
    manager->num_available_tasks++; 
    manager->num_available_results++; 
    manager->rear = (manager->rear + 1) % manager->capacity;
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != 0) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        return ERROR; 
    }
    
    // Signal to the writer that a result is available
    if (pthread_cond_signal(manager->results_available) != 0) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
        return ERROR; 
    }
        
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to release lock\n");
        return ERROR;
    }
    
    // Successfully added tasks
    return SUCCESS;
}

int force_termination(task_manager_t* manager) {
    
    // Acquire the lock
    if (pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Error: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Free all tasks and set them to NULL 
    for (size_t t = 0; t < manager->capacity; t++) {
        if (manager->tasks[t]) free_task(manager->tasks[t]); 
        manager->tasks[t] = NULL;
    }
    
    // Signal to the workers that a task is available
    if (pthread_cond_signal(manager->tasks_available) != 0) {
        
        // Failed to signal condition variable
        fprintf(stderr, "Error: Failed to signal condition variable\n");
    }
        
    // Release the lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
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
    if (manager->lock) {
        pthread_mutex_destroy(manager->lock);
        free(manager->lock);
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

// Function to wait for all tasks to complete
int wait_for_completion(task_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to wait for completion: Invalid input\n");
        return ERROR;
    }
    
    // Acquire lock 
    if(pthread_mutex_lock(manager->lock) != 0) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to wait for completion: Failed to acquire lock\n");
        return ERROR;
    }
    
    // Wait for all tasks to complete
    while (manager->size > 0) {
        
        // Wait for tasks to become available
        if (pthread_cond_wait(manager->tasks_completed, manager->lock) != 0) {
            
            // Failed to wait for condition variable
            fprintf(stderr, "Error: Failed to wait for completion: Error waiting for tasks\n");
            return ERROR;
        }
    }
    
    // Release lock
    if (pthread_mutex_unlock(manager->lock) != 0) {
        
        // Failed to release lock
        fprintf(stderr, "Error: Failed to wait for completion: Error releasing lock\n");
        return ERROR;
    }
    
    // Successfully waited for completion
    return SUCCESS;
}