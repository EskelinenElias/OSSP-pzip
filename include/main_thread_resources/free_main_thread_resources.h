#ifndef FREE_MAIN_THREAD_RESOURCES_H
#define FREE_MAIN_THREAD_RESOURCES_H

#include <pthread.h>

#include "../file_manager/free_file_manager.h"
#include "../task_queue/free_task_queue.h"
#include "../result_queue/free_result_queue.h"
#include "../worker_thread_pool/free_worker_thread_pool.h"
#include "../writer_thread/free_writer_thread.h"

#include "main_thread_resources.h"

// Function to free main_thread variables
int free_main_thread_resources(main_thread_resources_t* main_thread); 

#endif // FREE_MAIN_THREAD_RESOURCES_H
