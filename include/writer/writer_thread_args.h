#ifndef WRITER_THREAD_ARGS_H
#define WRITER_THREAD_ARGS_H

#include <stdio.h>

#include "../results_queue/results_queue.h"
#include "../file_manager/file_manager.h"

#include "writer_thread.h"

// Structure to represent writer thread input arguments
typedef struct {
    writer_thread_t* writer;
    results_queue_t* results_queue;
    file_manager_t* file_manager;
} writer_thread_args_t;

#endif // WRITER_THREAD_ARGS_H