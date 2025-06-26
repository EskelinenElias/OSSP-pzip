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
    pthread_mutex_t* lock; 
    size_t rear, size, capacity; 
    size_t next_task_index, next_result_index; 
    size_t num_available_tasks; 
    pthread_cond_t* room_available; 
    pthread_cond_t* tasks_completed; 
    pthread_cond_t* tasks_available;
    pthread_cond_t* next_result_available;
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
int yield_result(task_manager_t* manager, result_t* result, result_data_t* result_data);

// Function to claim a result from the manager
result_data_t* claim_result(task_manager_t* manager);

// Function to wait for all tasks to complete
int wait_for_completion(task_manager_t* manager);

// Function to yield a termination task to the manager
int yield_termination_task(task_manager_t* manager); 

// Function to free memory allocated to task manager and destroy mutex lock and condition variables
void* free_manager(task_manager_t* manager);

int force_termination(task_manager_t* manager); 

#endif // QUEUE_H