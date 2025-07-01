#ifndef INIT_PROCESS_H
#define INIT_PROCESS_H

#include <pthread.h>

#include "../../include/process/process.h"
#include "../../include/process/free_process.h"
#include "../../include/file_manager/init_file_manager.h"
#include "../../include/tasks_queue/init_tasks_queue.h"
#include "../../include/results_queue/init_results_queue.h"
#include "../../include/worker/init_worker.h"
#include "../../include/writer/init_writer.h"

// Function to initialize process variables
process_vars_t* init_process(size_t num_workers, size_t num_files); 

#endif // INIT_PROCESS_H
