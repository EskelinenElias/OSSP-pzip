#ifndef CLAIM_RESULT_H
#define CLAIM_RESULT_H

#include "../../include/results_queue/results_queue.h"
#include "../../include/result_data/free_result_data.h"

// Function to claim result from results queue
result_data_t* claim_result(results_queue_t* results_queue); 

#endif // CLAIM_RESULT_H