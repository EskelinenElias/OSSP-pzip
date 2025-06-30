#ifndef PROCESS_H
#define PROCESS_H

#include <pthread.h>

#include "../../include/file_manager/file_manager.h"
#include "../../include/task_manager/task_manager.h"

// Structure that represents process variables
typedef struct {
    file_manager_t* file_manager;
    task_manager_t* task_manager;
    pthread_t** workers;
    size_t num_workers;
    pthread_t* writer; 
} process_vars_t;

#endif // PROCESS_H
