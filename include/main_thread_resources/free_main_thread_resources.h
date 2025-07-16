#ifndef FREE_MAIN_THREAD_RESOURCES_H
#define FREE_MAIN_THREAD_RESOURCES_H

#include <pthread.h>

#include "../file_manager/free_file_manager.h"
#include "../tasks_queue/free_tasks_queue.h"
#include "../results_queue/free_results_queue.h"
#include "../worker_thread_pool/free_worker_thread_pool.h"
#include "../writer_thread/free_writer_thread.h"
#include "../task/free_task.h"

#include "main_thread_resources.h"

// Function to free main_thread variables
int free_main_thread_resources(main_thread_resources_t* main_thread); 

#endif // FREE_MAIN_THREAD_RESOURCES_H
