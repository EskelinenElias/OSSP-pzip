#ifndef INIT_MAIN_THREAD_RESOURCES_H
#define INIT_MAIN_THREAD_RESOURCES_H

#include <pthread.h>

#include "../file_manager/init_file_manager.h"
#include "../task_queue/init_task_queue.h"
#include "../result_queue/init_result_queue.h"
#include "../worker_thread_pool/init_worker_thread_pool.h"
#include "../writer_thread/init_writer_thread.h"

#include "main_thread_resources.h"
#include "free_main_thread_resources.h"

// Function to initialize main_thread variables
main_thread_resources_t* init_main_thread_resources(size_t num_workers, size_t num_files); 

#endif // INIT_MAIN_THREAD_RESOURCES_H
