#ifndef FREE_FILE_MANAGER_H
#define FREE_FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/file_manager/file_manager.h"
#include "../../include/file_manager/unmap_next_file.h"

// Function to initialize file manager
void* free_file_manager(file_manager_t* file_manager); 

#endif // FREE_FILE_MANAGER_H