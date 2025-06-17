#include "../include/task_queue.h"

task_queue_t* create_task_queue(size_t capacity) {
    
    // Initialize task queue and check for errors
    task_queue_t* queue = malloc(sizeof(task_queue_t)); 
    if (queue == NULL) return NULL; 
    
    // Allocate memory for tasks and check for errors
    queue->tasks = (encoding_task_t**)malloc(capacity * sizeof(encoding_task_t*));
    if (queue->tasks == NULL) { 
        free(queue); 
        return NULL; 
    }

    // Set queue fields
    queue->capacity = capacity; // Maximum possible size of the queue
    queue->front = 0; // First element of the queue
    queue->rear = 0; // Last element of the queue
    queue->size = 0; // Current size of the queue
    
    // Initialize mutex lock and check for errors
    if (pthread_mutex_init(&queue->lock, NULL) != 0) {
        
        // Error; free allocated memory
        free(queue->tasks); 
        free(queue); 
        return NULL; 
    };
    
    // Initialize condition variables and check for errors
    if (pthread_cond_init(&queue->not_empty, NULL) != 0 || pthread_cond_init(&queue->not_full, NULL) != 0) { 
            
        // Error; free allocated memory and destroy mutex
        pthread_mutex_destroy(&queue->lock); 
        free(queue->tasks);
        free(queue); 
        return NULL; 
    }
    
    // Successfully created queue
    return queue; 
}

// Function to free the task queue
void free_task_queue(task_queue_t* queue) {
    if (queue == NULL) { return; }
    free(queue->tasks);
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
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