#ifndef WORKER_GROUP_H
#define WORKER_GROUP_H

#include <stdio.h>

#include "worker_thread.h"

// Structure to represent worker group
typedef struct {
    worker_thread_t** workers; 
    size_t num_workers; 
} worker_group_t;

#endif // WORKER_GROUP_H