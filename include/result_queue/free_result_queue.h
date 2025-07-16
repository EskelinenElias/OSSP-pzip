#ifndef FREE_RESULT_QUEUE_H
#define FREE_RESULT_QUEUE_H

#include <stdio.h>
#include <pthread.h>

#include "../encoding_result/free_encoding_result.h"

#include "result_queue.h"

// Function to free result queue
int free_result_queue(result_queue_t* result_queue);

#endif // FREE_RESULT_QUEUE_H