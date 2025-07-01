#ifndef FREE_TASKS_QUEUE_H
#define FREE_TASKS_QUEUE_H

#include <stdlib.h>

#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/task_data/free_task_data.h"

// Function to free tasks queue
int free_tasks_queue(tasks_queue_t* tasks_queue);

#endif // FREE_TASKS_QUEUE_H