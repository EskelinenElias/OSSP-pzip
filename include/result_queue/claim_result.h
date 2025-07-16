#ifndef CLAIM_RESULT_H
#define CLAIM_RESULT_H

#include "../encoding_result/free_encoding_result.h"

#include "result_queue.h"

// Function to claim encoding result from result queue
encoding_result_t* claim_result(result_queue_t* result_queue); 

#endif // CLAIM_RESULT_H