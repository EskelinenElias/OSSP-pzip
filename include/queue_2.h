#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../include/encoding_task.h"

// Task queue
typedef struct {
    pthread_mutex_t lock;
    size_t index, num_tasks; 
    encoding_task_t** tasks; 
    pthread_cond_t tasks_available, tasks_completed;
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