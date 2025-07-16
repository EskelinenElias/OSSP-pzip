#ifndef INIT_ENCODING_TASK_H
#define INIT_ENCODING_TASK_H

#include <stdlib.h>

#include "../result_queue/reserve_spot.h"

#include "encoding_task.h"

// Function to initialize encoding task
encoding_task_t* init_encoding_task(char* data, size_t size, result_queue_t* result_queue);

#endif // INIT_ENCODING_TASK_H