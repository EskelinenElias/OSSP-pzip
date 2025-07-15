#ifndef FREE_WRITER_THREAD_RESOURCES_H
#define FREE_WRITER_THREAD_RESOURCES_H

#include <stdio.h>

#include "../constants.h"

#include "../result/free_result.h"

#include "writer_thread.h"

// Function to free writer thread resources
int free_writer_thread_resources(writer_thread_t* writer); 

#endif // FREE_WRITER_THREAD_RESOURCES_H