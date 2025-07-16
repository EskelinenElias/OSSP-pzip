#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "../task_queue/task_queue.h"
#include "../result_queue/result_queue.h"
#include "../encoding_task/encoding_task.h"
#include "../encoding_result/encoding_result.h"

// Structure to represent worker thread
typedef struct {
    pthread_t thread;
    
    /* Thread input arguments */
    task_queue_t* task_queue;
    result_queue_t* result_queue;
    
    /* Thread resources */
    encoding_task_t* encoding_task; 
    encoding_result_t* encoding_result; 
} worker_thread_t;

#endif // WORKER_THREAD_H