#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "constants.h"
#include "main_thread_resources/init_main_thread_resources.h"
#include "main_thread_resources/free_main_thread_resources.h"
#include "file_manager/map_next_file.h"
#include "task/init_task.h"
#include "task/free_task.h"
#include "tasks_queue/yield_task.h"

// Main function 
int main(int argc, char *argv[]); 

#endif // MAIN_H