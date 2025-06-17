#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/map_file.h"
#include "../include/constants.h"
#include "../include/task_queue.h"
#include "../include/worker.h"
#include "../include/encode.h"
#include "../include/encoded_data.h"
#include "../include/write_to_output.h"

typedef struct {
    task_queue_t* queue;
    pthread_t* workers;
    size_t num_workers;
    encoding_task_t* tasks;
    size_t num_tasks;
    encoding_task_t* boundary_task; 
    mapped_file_t file; 
} cleanup_args_t;

typedef struct {
    pthread_t* threads; 
    size_t num_threads;
} workers_list_t;

typedef struct {
    encoding_task_t* tasks;
    size_t num_tasks;
} tasks_list_t; 

// Main function 
int main(int argc, char *argv[]); 

#endif // MAIN_H