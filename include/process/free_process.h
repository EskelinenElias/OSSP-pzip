#ifndef FREE_PROCESS_H
#define FREE_PROCESS_H

#include <pthread.h>

#include "../../include/process/process.h"
#include "../../include/file_manager/free_file_manager.h"
#include "../../include/tasks_queue/free_tasks_queue.h"
#include "../../include/results_queue/free_results_queue.h"
#include "../../include/worker/terminate_workers.h"
#include "../../include/writer/terminate_writer.h"

// Function to free process variables
int free_process(process_vars_t* process); 

#endif // FREE_PROCESS_H
