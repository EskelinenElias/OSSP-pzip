#ifndef CLAIM_RESULT_H
#define CLAIM_RESULT_H

#include "../result/free_result.h"

#include "./results_queue.h"

// Function to claim result from results queue
result_t* claim_result(results_queue_t* results_queue); 

#endif // CLAIM_RESULT_H