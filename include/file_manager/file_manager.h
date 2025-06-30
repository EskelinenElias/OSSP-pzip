#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/file_manager/mapped_file.h"

// File manager structure; manages mapped files
typedef struct file_manager_t {
    pthread_mutex_t* lock; 
    size_t front, rear, size, capacity;  
    mapped_file_t** mapped_file_queue; 
} file_manager_t;

#endif // FILE_MANAGER_H