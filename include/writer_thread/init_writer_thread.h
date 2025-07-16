#ifndef INIT_WRITER_H
#define INIT_WRITER_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/results_queue/results_queue.h"

#include "writer_thread.h"
#include "writer_thread_main.h"

// Function to initialize writer thread
writer_thread_t* init_writer_thread(results_queue_t* results_queue, file_manager_t* file_manager); 

#endif // INIT_WRITER_H