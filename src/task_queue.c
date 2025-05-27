#include "../include/task_queue.h"

task_queue_t create_task_queue(size_t capacity) {
    
    // Initialize task queue
    task_queue_t queue; 
    
    // Allocate memory for the tasks in the queue
    queue.encoding_tasks = (encoding_task_t*)malloc(capacity * sizeof(encoding_task_t));
    if (queue.encoding_tasks == NULL) { // Check if memory allocation succeeded
        fprintf(stderr, "create_task_queue: Memory allocation failed!\n");
        exit(1); 
    }
    
    // Set queue attributes
    queue.capacity = capacity; // Maximum possible size of the queue
    queue.front = 0; // First element of the queue
    queue.rear = 0; // Last element of the queue
    queue.size = 0; // Current size of the queue
    
    // Initialize mutex lock 
    if (pthread_mutex_init(&queue.lock, NULL) != 0) {
        
        // Failed to initialize mutex lock 
        fprintf(stderr, "create_task_queue: Mutex initialization failed!\n");
        free(queue.encoding_tasks); // Free allocated memory before exiting
        exit(1);
    };
    
    // Initialize condition variables
    if (pthread_cond_init(&queue.not_empty, NULL) != 0 || pthread_cond_init(&queue.not_full, NULL) != 0) { 
            
        // Failed to initialize condition variables
        fprintf(stderr, "create_task_queue: Condition variable initialization failed!\n");
        pthread_mutex_destroy(&queue.lock); // Destroy mutex before exiting
        free(queue.encoding_tasks); // Free allocated memory before exiting
        exit(1);
    }
    
    return queue; 
}

// Function to free the task queue
void free_task_queue(task_queue_t* queue) {
    if (queue == NULL) { return; }
    free(queue->encoding_tasks);
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
}

encoding_task_t get_task(task_queue_t *queue) {
    
    // Acquire lock
    pthread_mutex_lock(&queue->lock);
    
    // Wait until the queue is not empty
    while (queue->size == 0) { 
        pthread_cond_wait(&queue->not_empty, &queue->lock);
    }
    
    // Get a task from the queue
    encoding_task_t encoding_task = queue->encoding_tasks[queue->front];
    queue->front = (queue->front + 1) % queue->capacity; // Move head
    queue->size--; // 
    
    // Signal to the main thread that space is available
    pthread_cond_signal(&queue->not_full); 
    
    // Release the lock and return the task 
    pthread_mutex_unlock(&queue->lock);
    return encoding_task;
}

// Function to add a new task to the task queue
int add_task(task_queue_t *queue, encoding_task_t encoding_task) {
    
    // Input validation
    if (queue == NULL) {
        fprintf(stderr, "add_task: Invalid input.\n"); 
        return 1; // Error
    }
    
    // Acquire lock 
    pthread_mutex_lock(&queue->lock);
    
    // Wait until there is room in the queue
    while (queue->size == queue->capacity) { 
        if (pthread_cond_wait(&queue->not_full, &queue->lock) != 0) {
            fprintf(stderr, "add_task: Failed to wait for room in the queue.\n"); 
            return 1; // Error
        }
    }
    
    // Add task to the end of the queue
    queue->encoding_tasks[queue->rear] = encoding_task;
    queue->rear = (queue->rear + 1) % queue->capacity; // Move tail
    queue->size++;
    
    // Signal workers that a task is available and release lock
    int result_signal = pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->lock);
    if (result_signal != 0) {
        fprintf(stderr, "add_task: Failed to signal workers.\n"); 
        return 1; // Error
    }
    return 0; // Success
}

// Function to terminate all threads; sends termination signal, then waits for threads to finish
void terminate_threads(task_queue_t* queue, pthread_t* thread, int num_threads) {
    
    // Send termination signal to all threads
    for (int i = 0; i < num_threads; i++) {
        
        // Create termination task
        encoding_task_t termination_task = get_termination_task(); 
        
        // Add termination task to the queue
        add_task(queue, termination_task);
    } 
    
    // Wait until all threads have terminated
    for (int i = 0; i < num_threads; i++) {
        
        // 
        pthread_join(*thread, NULL);
        thread++; 
    }
}