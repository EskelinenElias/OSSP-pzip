#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../include/encoding_task.h"

// Task queue
typedef struct {
    size_t capacity; 
    size_t front, rear, size;
    pthread_mutex_t lock;
    pthread_cond_t not_empty, not_full;
    encoding_task_t** tasks;
} task_queue_t;

// Function to create a task queue
task_queue_t* create_task_queue(size_t capacity);

// Function to free memory allocated to task queue and destroy mutex lock and condition variables
void free_task_queue(task_queue_t* queue);

// Function to get a task from a task queue
encoding_task_t* get_task(task_queue_t *queue);

// Function to add a task to a task queue
int add_task(task_queue_t* queue, encoding_task_t* encoding_task);

// Function to add termination tasks to task queue to terminate all threads
void terminate_threads(task_queue_t* queue, pthread_t* thread, int num_threads);

#endif // QUEUE_H