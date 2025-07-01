#ifndef INIT_TASKS_QUEUE_H
#define INIT_TASKS_QUEUE_H

#include "../../include/tasks_queue/tasks_queue.h"
#include "../../include/tasks_queue/free_tasks_queue.h"

// Function to initialize tasks queue
tasks_queue_t* init_tasks_queue(size_t capacity);

#endif // INIT_TASKS_QUEUE_H