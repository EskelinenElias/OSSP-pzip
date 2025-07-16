#ifndef YIELD_NULL_RESULT_H
#define YIELD_NULL_RESULT_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "result_queue.h"

// Function to yield NULL result to result queue
int yield_NULL_result(result_queue_t* result_queue); 

#endif // YIELD_NULL_RESULT_H