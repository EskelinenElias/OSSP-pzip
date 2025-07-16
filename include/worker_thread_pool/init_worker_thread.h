#ifndef INIT_WORKER_THREAD_H
#define INIT_WORKER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "../tasks_queue/tasks_queue.h"
#include "../results_queue/results_queue.h"

#include "worker_thread.h"
#include "worker_thread_main.h"

// Function to initialize worker thread
worker_thread_t* init_worker_thread(tasks_queue_t* tasks_queue, results_queue_t* results_queue); 

#endif // INIT_WORKER_THREAD_H