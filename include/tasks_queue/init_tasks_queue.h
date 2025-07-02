#ifndef INIT_TASKS_QUEUE_H
#define INIT_TASKS_QUEUE_H

#include "tasks_queue.h"
#include "free_tasks_queue.h"

// Function to initialize tasks queue
tasks_queue_t* init_tasks_queue(size_t capacity);

#endif // INIT_TASKS_QUEUE_H