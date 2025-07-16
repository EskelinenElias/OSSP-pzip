#ifndef FREE_TASK_QUEUE_H
#define FREE_TASK_QUEUE_H

#include <stdlib.h>

#include "task_queue.h"
#include "free_task.h"

// Function to free task queue
int free_task_queue(task_queue_t* task_queue);

#endif // FREE_TASK_QUEUE_H