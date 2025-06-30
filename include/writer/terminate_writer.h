#ifndef TERMINATE_WRITER_H
#define TERMINATE_WRITER_H

#include <pthread.h>

#include "../../include/task_manager/task_manager.h"

// Function to terminate the writer thread
void* terminate_writer(pthread_t* writer, task_manager_t* task_manager); 

#endif // TERMINATE_WRITER_H