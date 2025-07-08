#ifndef WRITER_LOOP_H
#define WRITER_LOOP_H

#include <stdio.h>
#include <pthread.h>

#include "../constants.h"
#include "../result/result.h"
#include "../file_manager/file_manager.h"
#include "../file_manager/unmap_next_file.h"
#include "../results_queue/results_queue.h"
#include "../results_queue/claim_result.h"
#include "../result/free_result.h"

#include "handle_boundary.h"
#include "write_to_output.h"

// Structure to represent writer resources
typedef struct {
    result_t* current_result;
    result_t* next_result;
} writer_resources_t;

// Structure to represent writer arguments
typedef struct {
    file_manager_t* file_manager;
    results_queue_t* results_queue;
} writer_args_t;

// Function to initialize writer resources
writer_resources_t* init_writer_resources();

// Function to free writer resources
int free_writer_resources(writer_resources_t* resources);

// Function to process results (in a writer thread)
void* process_results(void* args); 

#endif // WRITER_LOOP_H