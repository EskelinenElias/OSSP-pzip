#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/constants.h"
#include "../include/process/init_process.h"
#include "../include/process/free_process.h"
#include "../include/file_manager/map_next_file.h"
#include "../include/task_manager/yield_task_data.h"

// Main function 
int main(int argc, char *argv[]); 

#endif // MAIN_H