#ifndef FREE_MAPPED_FILE_H   
#define FREE_MAPPED_FILE_H   

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "../constants.h"

#include "mapped_file.h"

// Function to free mapped file (a memory mapped file)
int free_mapped_file(mapped_file_t* mapped_file);

#endif // FREE_MAPPED_FILE_H