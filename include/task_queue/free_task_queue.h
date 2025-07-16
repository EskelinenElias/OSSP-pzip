#ifndef FREE_TASK_QUEUE_H
#define FREE_TASK_QUEUE_H

#include <stdlib.h>

#include "../encoding_task/free_encoding_task.h"

#include "task_queue.h"

// Function to free task queue
int free_task_queue(task_queue_t* task_queue);

#endif // FREE_TASK_QUEUE_H