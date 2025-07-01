#ifndef INIT_WORKER_H
#define INIT_WORKER_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/results_queue/results_queue.h"
#include "../../include/worker/process_tasks.h"

// Function to initialize a worker thread
pthread_t* init_worker(tasks_queue_t* tasks_queue, results_queue_t* results_queue); 

#endif // INIT_WORKER_H