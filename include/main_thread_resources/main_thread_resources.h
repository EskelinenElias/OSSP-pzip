#ifndef MAIN_THREAD_RESOURCES_H
#define MAIN_THREAD_RESOURCES_H

#include <pthread.h>

#include "../file_manager/file_manager.h"
#include "../task_queue/task_queue.h"
#include "../result_queue/result_queue.h"
#include "../worker_thread_pool/worker_thread_pool.h"
#include "../writer_thread/writer_thread.h"
#include "../task_queue/task.h"

// Structure to represent main thread resources
typedef struct {
    file_manager_t* file_manager;
    task_queue_t* task_queue;
    result_queue_t* result_queue; 
    worker_thread_pool_t* worker_pool;
    writer_thread_t* writer; 
    task_t* task; 
} main_thread_resources_t;

#endif // MAIN_THREAD_RESOURCES_H
