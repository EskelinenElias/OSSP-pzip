#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "../tasks_queue/tasks_queue.h"
#include "../results_queue/results_queue.h"
#include "../task/task.h"
#include "../result/result.h"

// Structure to represent worker thread
typedef struct {
    pthread_t thread;
    
    /* Thread input arguments */
    tasks_queue_t* tasks_queue;
    results_queue_t* results_queue;
    
    /* Thread resources */
    task_t* task; 
    result_t* result; 
} worker_thread_t;

#endif // WORKER_THREAD_H