#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task_manager/task.h"
#include "../../include/task_manager/result.h"

// Task manager
typedef struct {
    pthread_mutex_t* lock; 
    size_t rear, size, capacity; 
    size_t tasks_front, results_front; 
    size_t num_available_tasks; 
    pthread_cond_t* room_available; 
    pthread_cond_t* tasks_completed; 
    pthread_cond_t* tasks_available;
    pthread_cond_t* next_result_available;
    task_t** tasks_queue; 
    result_t** results_queue; 
} task_manager_t;

// Function to initialize a task manager with a given capacity
task_manager_t* init_manager(size_t capacity);

// Function to free memory allocated to task manager and destroy mutex lock and condition variables
void* free_manager(task_manager_t* manager);

// Function to yield a termination task to the manager
int send_termination_signal(task_manager_t* manager); 

// int force_termination(task_manager_t* manager); 

#endif // TASK_MANAGER_H