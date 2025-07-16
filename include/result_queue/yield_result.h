#ifndef YIELD_RESULT_H
#define YIELD_RESULT_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "result_queue.h"

// Function to yield result to result queue
int yield_result(result_queue_t* result_queue, encoding_result_t* encoding_result, size_t reserved_index); 

#endif // YIELD_RESULT_H