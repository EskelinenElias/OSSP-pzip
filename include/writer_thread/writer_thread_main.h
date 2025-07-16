#ifndef WRITER_THREAD_MAIN_H
#define WRITER_THREAD_MAIN_H

#include <stdio.h>
#include <pthread.h>

#include "../constants.h"
#include "../file_manager/file_manager.h"
#include "../file_manager/unmap_next_file.h"
#include "../result_queue/result_queue.h"
#include "../result_queue/result.h"
#include "../result_queue/free_result.h"
#include "../result_queue/claim_result.h"

#include "writer_thread.h"
#include "handle_boundary.h"
#include "write_to_output.h"

// Writer thread main function (writer thread processes result in result queue)
void* writer_thread_main(void* args); 

#endif // WRITER_THREAD_MAIN_H