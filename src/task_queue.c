#include "../include/queue_2.h"

task_queue_t* init_queue() {
    
    // Initialize task queue and check for errors
    task_queue_t* queue = malloc(sizeof(task_queue_t)); 
    if (!queue) {
        
        // Error; failed to allocate memory for task queue
        fprintf(stderr, "Error: Failed to allocate memory for task queue\n");
        return NULL; 
    }    

    // Set queue fields
    queue->tasks = NULL; 
    queue->index = 0; // Index to the current task in the queue
    queue->num_tasks = 0; // Number of tasks in the queue

    // Initialize mutex lock and check for errors
    if (pthread_mutex_init(&queue->lock, NULL) != 0) {
        
        // Error; free allocated memory
        free(queue); 
        return NULL; 
    };
    
    // Initialize condition variable for signaling tasks are available and check for errors
    if (pthread_cond_init(&queue->tasks_available, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        pthread_mutex_destroy(&queue->lock); 
        free(queue); 
        return NULL; 
    }
    
    // Successfully created queue
    return queue; 
}

int add_tasks(task_queue_t* queue, encoding_task_t** tasks, size_t num_tasks) {
    
    // Input validation
    if (queue == NULL || tasks == NULL || num_tasks == 0) return FAILURE; 
    
    // Acquire lock
    pthread_mutex_lock(&queue->lock);
    
    // Add tasks 
    queue->tasks = tasks; 
    queue->num_tasks = num_tasks;
    queue->index = 0; 
    
    // Signal to the main thread that tasks are available
    pthread_cond_signal(&queue->tasks_available);
    
    // Release the lock
    pthread_mutex_unlock(&queue->lock);
    
    // Successfully added tasks
    return SUCCESS;
}

// Function to claim a task from the queue
encoding_task_t* claim_task(task_queue_t* queue) {
    
    // Input validation
    if (queue == NULL) return NULL;
    
    // Acquire lock
    pthread_mutex_lock(&queue->lock);
    
    // If there are no tasks available, wait for tasks to become available
    while (queue->index == queue->num_tasks) {
        pthread_cond_wait(&queue->tasks_available, &queue->lock);
    }
    
    // Claim a task from the queue
    encoding_task_t* task = queue->tasks[queue->index++];
    
    // Release the lock
    pthread_mutex_unlock(&queue->lock);
    
    // Return claimed task
    return task;
}

// Function to free the task queue
void free_queue(task_queue_t* queue) {
    
    // Input validation
    if (queue == NULL) { return; }
    
    // Destroy mutex lock
    pthread_mutex_destroy(&queue->lock);
    
    // Destroy condition variable
    pthread_cond_destroy(&queue->tasks_available);
    
    // Free memory
    free(queue); 
}

encoding_task_t* get_task(task_queue_t *queue) {
        
    // Acquire lock
    pthread_mutex_lock(&queue->lock);
    
    // Wait until the queue is not empty
    while (queue->size == 0) pthread_cond_wait(&queue->not_empty, &queue->lock);
    
    // Get a task from the queue
    encoding_task_t* task = queue->tasks[queue->front];
    queue->tasks[queue->front] = NULL;
    queue->front = (queue->front + 1) % queue->capacity; 
    queue->size--; 
    
    // Release the lock and return the task 
    pthread_mutex_unlock(&queue->lock);
    
    // Signal to the main thread that space is available
    pthread_cond_signal(&queue->not_full);
    return task;
}

// Function to add a new task to the task queue
int add_task(task_queue_t *queue, encoding_task_t* task) {
    
    // Input validation
    if (queue == NULL) return ERROR; // NULL task stands for termination signal
    
    // Acquire lock 
    pthread_mutex_lock(&queue->lock);
    
    // Wait until there is room in the queue
    while (queue->size == queue->capacity) { 
        if (pthread_cond_wait(&queue->not_full, &queue->lock) != 0) return ERROR;
    }
    
    // Add task to the end of the queue
    queue->tasks[queue->rear] = task;
    queue->rear = (queue->rear + 1) % queue->capacity; // Move tail
    queue->size++;
    
    // Signal workers that a task is available and release lock
    if (pthread_cond_signal(&queue->not_empty) != 0) {
        fprintf(stderr, "add_task: Failed to signal workers.\n"); 
        return FAILURE; // Error
    }

    pthread_mutex_unlock(&queue->lock);
    return SUCCESS; // Success
}

// Function to terminate all threads; sends termination signal, then waits for threads to finish
void terminate_threads(task_queue_t* queue, pthread_t* thread, int num_threads) {
    
    // Send termination signal to all threads
    for (int i = 0; i < queue->capacity; i++) {
        
        // Add termination task to the queue
        add_task(queue, NULL);
    } 
    
    // Wait until all threads have terminated
    for (int i = 0; i < num_threads; i++) {
        
        // Wait for thread to finish
        pthread_join(*thread, NULL);
        thread++; 
    }
}