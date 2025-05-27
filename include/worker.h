#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include "../include/constants.h"
#include "../include/encode.h"
#include "../include/encoded_data.h"
#include "../include/task_queue.h"
#include "../include/encoding_task.h"

// Function to process the input in threads
void* thread_worker(void* args); 

#endif // WORKER_H