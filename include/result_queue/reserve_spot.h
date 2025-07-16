#ifndef RESERVE_SPOT_H
#define RESERVE_SPOT_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "../constants.h"

#include "result_queue.h"
#include "free_result.h"

// Function to reserve a spot in the result queue
size_t reserve_spot(result_queue_t* result_queue); 

#endif // RESERVE_SPOT_H