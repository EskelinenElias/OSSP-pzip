#ifndef MAP_NEXT_FILE_H
#define MAP_NEXT_FILE_H

#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

#include "../../include/constants.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/file_manager/mapped_file.h"

// Function to map a file into memory
mapped_file_t* map_next_file(file_manager_t* file_manager, const char *filepath); 

#endif // MAP_NEXT_FILE_H