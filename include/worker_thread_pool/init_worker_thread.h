#ifndef INIT_WORKER_THREAD_H
#define INIT_WORKER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "../task_queue/task_queue.h"
#include "../result_queue/result_queue.h"

#include "worker_thread.h"
#include "worker_thread_main.h"

// Function to initialize worker thread
worker_thread_t* init_worker_thread(task_queue_t* task_queue, result_queue_t* result_queue); 

#endif // INIT_WORKER_THREAD_H