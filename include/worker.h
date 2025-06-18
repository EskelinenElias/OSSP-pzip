#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include "../include/constants.h"
#include "../include/encode.h"
#include "../include/task_manager.h"
#include "../include/task.h"
#include "../include/result.h"

// Function to process the input in threads
void* thread_worker(void* args); 

// Function to initialize worker threads
pthread_t* init_workers(size_t num_workers, task_manager_t *manager); 

int terminate_workers(pthread_t *threads, size_t num_workers, task_manager_t *queue);

#endif // WORKER_H