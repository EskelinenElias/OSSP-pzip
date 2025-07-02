#ifndef RESERVE_SPOT_H
#define RESERVE_SPOT_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/results_queue/results_queue.h"
#include "../../include/result/free_result.h"

// Function to reserve a spot in the results queue
size_t reserve_spot(results_queue_t* results_queue); 

#endif // RESERVE_SPOT_H