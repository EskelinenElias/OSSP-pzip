#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/mmap_file.h"
#include "../include/constants.h"
#include "../include/task_queue.h"
#include "../include/worker.h"
#include "../include/encode.h"
#include "../include/encoded_data.h"
#include "../include/write_to_output.h"

// main function 
int main(int argc, char *argv[]); 

#endif // MAIN_H