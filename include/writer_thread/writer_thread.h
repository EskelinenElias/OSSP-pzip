#ifndef WRITER_THREAD_H
#define WRITER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "../results_queue/results_queue.h"
#include "../file_manager/file_manager.h"
#include "../result/result.h"

// Structure to represent writer thread resources
typedef struct {
    
    /* Thread */
    pthread_t thread;
    
    /* Thread input arguments */
    results_queue_t* results_queue;
    file_manager_t* file_manager;
    
    /* Thread resources */
    result_t* current_result;
    result_t* next_result;
    
} writer_thread_t;

#endif // WRITER_THREAD_H