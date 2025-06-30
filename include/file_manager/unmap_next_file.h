#ifndef UNMAP_NEXT_FILE_H
#define UNMAP_NEXT_FILE_H

#include "../../include/constants.h"
#include "../../include/file_manager/file_manager.h"
#include "../../include/file_manager/mapped_file.h"

// Function to map a file into memory
int unmap_next_file(file_manager_t* file_manager); 

#endif // UNMAP_NEXT_FILE_H