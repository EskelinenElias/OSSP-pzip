#ifndef WRITER_THREAD_H
#define WRITER_THREAD_H

#include <stdio.h>
#include <pthread.h>

#include "writer_thread_resources.h"

// Structure to represent writer thread
typedef struct {
    pthread_t thread;
    writer_thread_resources_t* resources;
} writer_thread_t;

#endif // WRITER_THREAD_H