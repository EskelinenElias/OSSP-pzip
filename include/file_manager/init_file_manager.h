#ifndef INIT_FILE_MANAGER_H
#define INIT_FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/constants.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/file_manager/mapped_file.h"

// Function to initialize file manager
file_manager_t* init_file_manager(size_t capacity); 

#endif // INIT_FILE_MANAGER_H