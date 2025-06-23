#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/map_file.h"
#include "../include/constants.h"
#include "../include/task_manager.h"
#include "../include/worker.h"
#include "../include/encode.h"
#include "../include/encoded_data.h"
#include "../include/write_to_output.h"

typedef struct {
    task_manager_t* manager;
    pthread_t* workers;
    size_t num_workers;
    result_t* current_result;
    result_t* next_result; 
    mapped_file_t** files; 
    size_t num_files;
} process_vars_t;

typedef struct {
    pthread_t* threads; 
    size_t num_threads;
} workers_list_t;

typedef struct {
    task_t* tasks;
    size_t num_tasks;
} tasks_list_t; 

// Main function 
int main(int argc, char *argv[]); 

#endif // MAIN_H