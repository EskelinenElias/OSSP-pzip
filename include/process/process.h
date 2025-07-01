#ifndef PROCESS_H
#define PROCESS_H

#include <pthread.h>

#include "../../include/file_manager/file_manager.h"
#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/results_queue/results_queue.h"

// Structure that represents process variables
typedef struct {
    file_manager_t* file_manager;
    tasks_queue_t* tasks_queue;
    results_queue_t* results_queue; 
    pthread_t** workers;
    size_t num_workers;
    pthread_t* writer; 
} process_vars_t;

#endif // PROCESS_H
