#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/constants.h"
#include "../include/file_manager/file_manager.h"
#include "../include/task_manager/init_task_manager.h"
#include "../include/task_manager/yield_task_data.h"
#include "../include/worker/init_worker.h"
#include "../include/writer/writer.h"

typedef struct {
    file_manager_t* file_manager;
    task_manager_t* task_manager;
    pthread_t** workers;
    size_t num_workers;
    pthread_t* writer; 
} process_vars_t;

// Function to initialize process variables
process_vars_t* init_process(size_t num_workers, size_t num_files); 

// Function to free process variables
process_vars_t* free_process(process_vars_t* process); 

// Main function 
int main(int argc, char *argv[]); 

#endif // MAIN_H