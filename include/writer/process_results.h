#ifndef WRITER_LOOP_H
#define WRITER_LOOP_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/result_data/result_data.h"
#include "../../include/writer/handle_boundary.h"
#include "../../include/writer/write_to_output.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/file_manager/unmap_next_file.h"
#include "../../include/results_queue/results_queue.h"
#include "../../include/results_queue/claim_result.h"
#include "../../include/result_data/free_result_data.h"

// Structure to hold arguments for writer thread
typedef struct {
    file_manager_t* file_manager;
    results_queue_t* results_queue;
} writer_args_t;

// Function to process the input in threads
void* process_results(void* args); 

#endif // WRITER_LOOP_H