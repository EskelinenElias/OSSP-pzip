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

#include "file_manager.h"

// Function to free file manager
int free_file_manager(file_manager_t* file_manager); 

#endif // FREE_FILE_MANAGER_H