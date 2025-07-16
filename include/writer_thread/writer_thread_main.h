#ifndef WRITER_THREAD_MAIN_H
#define WRITER_THREAD_MAIN_H

#include <stdio.h>
#include <pthread.h>

#include "../constants.h"
#include "../result/result.h"
#include "../file_manager/file_manager.h"
#include "../file_manager/unmap_next_file.h"
#include "../results_queue/results_queue.h"
#include "../results_queue/claim_result.h"
#include "../result/free_result.h"

#include "writer_thread.h"
#include "handle_boundary.h"
#include "write_to_output.h"

// Writer thread main function (writer thread processes results in results queue)
void* writer_thread_main(void* args); 

#endif // WRITER_THREAD_MAIN_H