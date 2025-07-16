#ifndef CLAIM_RESULT_H
#define CLAIM_RESULT_H

#include "result_queue.h"
#include "free_result.h"

// Function to claim result from result queue
result_t* claim_result(result_queue_t* result_queue); 

#endif // CLAIM_RESULT_H