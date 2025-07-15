#ifndef FREE_WORKER_GROUP_H
#define FREE_WORKER_GROUP_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../tasks_queue/tasks_queue.h"
#include "../tasks_queue/yield_task.h"

#include "worker_group.h"
#include "free_worker_thread.h"

// Function to free workers
int free_worker_group(worker_group_t* group, tasks_queue_t* tasks_queue); 

#endif // FREE_WORKER_GROUP_H