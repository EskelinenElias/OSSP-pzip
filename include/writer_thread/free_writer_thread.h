#ifndef FREE_WRITER_THREAD_H
#define FREE_WRITER_THREAD_H

#include <pthread.h>

#include "../result_queue/reserve_spot.h"
#include "../result_queue/yield_result.h"

#include "writer_thread.h"

// Function to free writer thread
int free_writer_thread(writer_thread_t* writer_thread); 

#endif // FREE_WRITER_THREAD_H