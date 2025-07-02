#ifndef UNMAP_NEXT_FILE_H
#define UNMAP_NEXT_FILE_H

#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

#include "../constants.h"

#include "file_manager.h"
#include "mapped_file.h"

// Function to map a file into memory
int unmap_next_file(file_manager_t* file_manager); 

#endif // UNMAP_NEXT_FILE_H