#ifndef INIT_MAPPED_FILE_H   
#define INIT_MAPPED_FILE_H   

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "mapped_file.h"

// Function to initialize a mapped file (a memory mapped file)
mapped_file_t* init_mapped_file(int file_descriptor, struct stat* file_stat);

#endif // INIT_MAPPED_FILE_H