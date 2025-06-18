#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../include/constants.h"
#include "../include/task.h"
#include "../include/result.h"

// Task manager
typedef struct {
    pthread_mutex_t* tasks_lock; 
    size_t next_task, next_result, next_slot, current_size, capacity;  
    pthread_cond_t* room_available; 
    pthread_cond_t* tasks_available;
    pthread_cond_t* results_available;
    task_t** tasks; 
    result_t** results; 
} task_manager_t;

// Function to initialize a task manager with a given capacity
task_manager_t* init_manager(size_t capacity);

// Function to add tasks to the task manager
int yield_task(task_manager_t* manager, char* data, size_t length); 

// Function to claim a task from the manager
task_t* claim_task(task_manager_t* manager); 

// Function to yield results to the manager
int yield_result(task_manager_t* manager, result_t* result, size_t index);

// Function to claim a result from the manager
result_t* claim_result(task_manager_t* manager);

// Function to yield a termination task to the manager
int yield_termination_task(task_manager_t* manager); 

// Function to free memory allocated to task manager and destroy mutex lock and condition variables
void free_manager(task_manager_t* manager);

#endif // QUEUE_H