#ifndef FREE_PROCESS_H
#define FREE_PROCESS_H

#include <pthread.h>

#include "../../include/process/process.h"
#include "../../include/file_manager/free_file_manager.h"
#include "../../include/task_manager/free_task_manager.h"
#include "../../include/writer/terminate_writer.h"

// Function to free process variables
void* free_process(process_vars_t* process); 

#endif // FREE_PROCESS_H
