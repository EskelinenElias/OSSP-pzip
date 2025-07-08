#ifndef FREE_WORKERS_H
#define FREE_WORKERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/tasks_queue/yield_task.h"

// Function to free workers
int free_workers(pthread_t** workers, size_t num_workers, tasks_queue_t* tasks_queue); 

#endif // FREE_WORKERS_H