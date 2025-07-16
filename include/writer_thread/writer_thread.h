#ifndef WRITER_THREAD_H
#define WRITER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "../result_queue/result_queue.h"
#include "../file_manager/file_manager.h"
#include "../encoding_result/encoding_result.h"

// Structure to represent writer thread resources
typedef struct {
    
    /* Thread */
    pthread_t thread;
    
    /* Thread input arguments */
    result_queue_t* result_queue;
    file_manager_t* file_manager;
    
    /* Thread resources */
    encoding_result_t* current_result;
    encoding_result_t* next_result;
    
} writer_thread_t;

#endif // WRITER_THREAD_H