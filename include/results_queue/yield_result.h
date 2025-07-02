#ifndef YIELD_RESULT_H
#define YIELD_RESULT_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/results_queue/results_queue.h"

// Function to yield result to results queue
int yield_result(results_queue_t* results_queue, result_t* result, size_t reserved_index); 

#endif // YIELD_RESULT_H