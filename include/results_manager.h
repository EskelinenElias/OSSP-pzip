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
    size_t front, rear, size, capacity; 
    pthread_cond_t* room_available; 
    pthread_cond_t* next_available;
    result_t** results_queue; 
} results_manager_t;

// Function to initialize a task manager with a given capacity
results_manager_t* init_results_manager(size_t capacity);

// Function to yield results to the manager
int yield_result_data(results_manager_t* results_manager, result_t* result, result_data_t* result_data);

// Function to claim a result from the manager
result_data_t* claim_result_data(results_manager_t* results_manager);

// Function to wait for all tasks to complete
int wait_for_completion(results_manager_t* manager);

// Function to yield a termination task to the manager
int yield_termination_task(results_manager_t* manager); 

// Function to free memory allocated to task manager and destroy mutex lock and condition variables
void* free_results_manager(results_manager_t* results_manager);

int force_termination(results_manager_t* manager); 

#endif // QUEUE_H