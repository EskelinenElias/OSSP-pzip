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

int init_workers(pthread_t *threads, size_t num_workers, task_queue_t *queue); 

int terminate_workers(pthread_t *threads, size_t num_workers, task_queue_t *queue);

#endif // WORKER_H