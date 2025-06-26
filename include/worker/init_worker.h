#ifndef INIT_WORKER_H
#define INIT_WORKER_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/worker/do_work.h"
#include "../../include/task_manager/task_manager.h"

// Function to initialize a worker thread
pthread_t* init_worker(task_manager_t* task_manager); 

#endif // INIT_WORKER_H