#ifndef INIT_TASK_H
#define INIT_TASK_H

#include <stdlib.h>

#include "../results_queue/reserve_spot.h"

#include "task.h"

// Function to initialize task
task_t* init_task(char* data, size_t size, results_queue_t* results_queue);

#endif // INIT_TASK_H