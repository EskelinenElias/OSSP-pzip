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

// Structure to hold arguments for writer thread
typedef struct {
    file_manager_t* file_manager;
    results_queue_t* results_queue;
} writer_args_t;

// Function to process the input in threads
void* process_results(void* args); 

#endif // WRITER_LOOP_H