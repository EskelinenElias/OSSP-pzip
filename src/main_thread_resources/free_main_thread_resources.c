#include "../../include/main_thread_resources/free_main_thread_resources.h"

// Function to free main thread resources
int free_main_thread_resources(main_thread_resources_t* resources) {

    // Free worker thread pool
    if (resources->worker_pool) free_worker_thread_pool(resources->worker_pool);

    // Free writer thread
    if (resources->writer) free_writer_thread(resources->writer);

    // Free task queue
    if (resources->task_queue) free_task_queue(resources->task_queue);

    // Free result queue
    if (resources->result_queue) free_result_queue(resources->result_queue);

    // Free file manager
    if (resources->file_manager) free_file_manager(resources->file_manager);

    // Free memory allocated for main thread resources
    free(resources);

    // Successfully freed main thread resources
    return SUCCESS;
}

// EOF
