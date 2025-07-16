#ifndef FREE_WORKER_THREAD_H
#define FREE_WORKER_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../constants.h"

#include "../task_queue/free_task.h"
#include "../result_queue/free_result.h"

#include "worker_thread.h"

// Function to free worker thread
int free_worker_thread(worker_thread_t* worker); 

#endif // FREE_WORKER_THREAD_H