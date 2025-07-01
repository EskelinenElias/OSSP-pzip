#ifndef TASKS_QUEUE_H
#define TASKS_QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/task_data/task_data.h"

// Task manager
typedef struct {
    pthread_mutex_t* lock; 
    size_t front, rear, size, capacity; 
    pthread_cond_t* room_available; 
    pthread_cond_t* tasks_available;
    task_data_t** tasks; 
} tasks_queue_t;

#endif // TASKS_QUEUE_H