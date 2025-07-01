#ifndef INIT_RESULTS_QUEUE_H
#define INIT_RESULTS_QUEUE_H

#include "../../include/results_queue/results_queue.h"
#include "../../include/results_queue/free_results_queue.h"

// Function to initialize results queue
results_queue_t* init_results_queue(size_t capacity);

#endif // INIT_RESULTS_QUEUE_H