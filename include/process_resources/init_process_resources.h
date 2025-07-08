#ifndef INIT_PROCESS_RESOURCES_H
#define INIT_PROCESS_RESOURCES_H

#include <pthread.h>

#include "../file_manager/init_file_manager.h"
#include "../tasks_queue/init_tasks_queue.h"
#include "../results_queue/init_results_queue.h"
#include "../worker/init_worker.h"
#include "../writer/init_writer.h"

#include "process_resources.h"
#include "free_process_resources.h"


// Function to initialize process variables
process_resources_t* init_process_resources(size_t num_workers, size_t num_files); 

#endif // INIT_PROCESS_RESOURCES_H
