#ifndef RESULTS_QUEUE_H
#define RESULTS_QUEUE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/result_data/result_data.h"

// Structure of the results queue
typedef struct {
    pthread_mutex_t* lock; 
    size_t front, rear, size, capacity; 
    pthread_cond_t* room_available; 
    pthread_cond_t* result_available;
    result_data_t** results; 
    int* statuses; 
} results_queue_t;

#endif // RESULTS_QUEUE_H