#ifndef FREE_WRITER_THREAD_H
#define FREE_WRITER_THREAD_H

#include <pthread.h>

#include "../results_queue/reserve_spot.h"
#include "../results_queue/yield_result.h"

#include "writer_thread.h"
#include "free_writer_thread_resources.h"

// Function to free writer thread
int free_writer_thread(writer_thread_t* writer_thread, results_queue_t* results_queue); 

#endif // FREE_WRITER_THREAD_H