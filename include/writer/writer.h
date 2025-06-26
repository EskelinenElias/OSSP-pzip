#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/writer/handle_boundary.h"
#include "../../include/writer/write_to_output.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/task_manager/task_manager.h"
#include "../../include/task_manager/claim_result_data.h"
#include "../../include/task_manager/result.h"

// Structure to hold arguments for writer thread
typedef struct {
    file_manager_t* file_manager;
    task_manager_t* task_manager;
} writer_args_t;

// Function to process the input in threads
void* writer(void* args); 

// Function to initialize worker threads
pthread_t* init_writer(file_manager_t* file_manager, task_manager_t* task_manager); 

// int terminate_writer(pthread_t *threads, size_t num_workers, task_manager_t *queue);

void* terminate_writer(pthread_t* writer, task_manager_t* task_manager); 

#endif // WRITER_H