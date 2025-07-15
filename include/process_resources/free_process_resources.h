#ifndef FREE_PROCESS_RESOURCES_H
#define FREE_PROCESS_RESOURCES_H

#include <pthread.h>

#include "../file_manager/free_file_manager.h"
#include "../tasks_queue/free_tasks_queue.h"
#include "../results_queue/free_results_queue.h"
#include "../worker/free_worker_group.h"
#include "../writer/free_writer_thread.h"

#include "process_resources.h"

// Function to free process variables
int free_process_resources(process_resources_t* process); 

#endif // FREE_PROCESS_RESOURCES_H
