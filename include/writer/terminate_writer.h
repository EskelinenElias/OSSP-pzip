#ifndef TERMINATE_WRITER_H
#define TERMINATE_WRITER_H

#include <pthread.h>

#include "../../include/results_queue/reserve_spot.h"
#include "../../include/results_queue/yield_result.h"

// Function to terminate writer thread
int terminate_writer(pthread_t* writer, results_queue_t* results_queue); 

#endif // TERMINATE_WRITER_H