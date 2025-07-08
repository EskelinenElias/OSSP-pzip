#ifndef FREE_WRITER_H
#define FREE_WRITER_H

#include <pthread.h>

#include "../../include/results_queue/reserve_spot.h"
#include "../../include/results_queue/yield_result.h"

// Function to free writer
int free_writer(pthread_t* writer, results_queue_t* results_queue); 

#endif // FREE_WRITER_H