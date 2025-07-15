#ifndef MAP_NEXT_FILE_H
#define MAP_NEXT_FILE_H

#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

#include "../constants.h"

#include "file_manager.h"
#include "init_mapped_file.h"

// Function to add a mapped file to the file manager or retrieve already mapped file
mapped_file_t* map_next_file(file_manager_t* file_manager, const char *filepath); 

#endif // MAP_NEXT_FILE_H