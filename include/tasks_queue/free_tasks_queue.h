#ifndef FREE_TASKS_QUEUE_H
#define FREE_TASKS_QUEUE_H

#include <stdlib.h>

#include "../task/free_task.h"

#include "tasks_queue.h"

// Function to free tasks queue
int free_tasks_queue(tasks_queue_t* tasks_queue);

#endif // FREE_TASKS_QUEUE_H