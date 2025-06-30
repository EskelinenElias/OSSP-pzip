#ifndef INIT_WRITER_H
#define INIT_WRITER_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/task_manager/task_manager.h"
#include "../../include/writer/writer_loop.h"

// Function to initialize the writer thread
pthread_t* init_writer(file_manager_t* file_manager, task_manager_t* task_manager); 

#endif // INIT_WRITER_H