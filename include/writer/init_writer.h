#ifndef INIT_WRITER_H
#define INIT_WRITER_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/results_queue/results_queue.h"
#include "../../include/writer/process_results.h"

// Function to initialize the writer thread
pthread_t* init_writer(file_manager_t* file_manager, results_queue_t* results_queue); 

#endif // INIT_WRITER_H