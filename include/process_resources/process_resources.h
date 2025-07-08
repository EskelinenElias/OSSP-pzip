#ifndef PROCESS_RESOURCES_H
#define PROCESS_RESOURCES_H

#include <pthread.h>

#include "../file_manager/file_manager.h"
#include "../tasks_queue/tasks_queue.h"
#include "../results_queue/results_queue.h"

// Structure to represent process resources
typedef struct {
    file_manager_t* file_manager;
    tasks_queue_t* tasks_queue;
    results_queue_t* results_queue; 
    pthread_t** workers;
    size_t num_workers;
    pthread_t* writer; 
} process_resources_t;

#endif // PROCESS_RESOURCES_H
