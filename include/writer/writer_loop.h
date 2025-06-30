#ifndef WRITER_LOOP_H
#define WRITER_LOOP_H

#include <stdio.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/writer/handle_boundary.h"
#include "../../include/writer/write_to_output.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/file_manager/unmap_next_file.h"
#include "../../include/task_manager/task_manager.h"
#include "../../include/task_manager/claim_result_data.h"
#include "../../include/task_manager/result.h"

// Structure to hold arguments for writer thread
typedef struct {
    file_manager_t* file_manager;
    task_manager_t* task_manager;
} writer_args_t;

// Function to process the input in threads
void* writer_loop(void* args); 

#endif // WRITER_LOOP_H