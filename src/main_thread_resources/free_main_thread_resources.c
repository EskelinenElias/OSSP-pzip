#include "../../include/main_thread_resources/free_main_thread_resources.h"

// Function to free main thread resources
int free_main_thread_resources(main_thread_resources_t* resources) {

    // Free worker thread pool
    if (resources->worker_pool) free_worker_thread_pool(resources->worker_pool);

    // Free writer thread
    if (resources->writer) free_writer_thread(resources->writer);

    // Free tasks queue
    if (resources->tasks_queue) free_tasks_queue(resources->tasks_queue);

    // Free results queue
    if (resources->results_queue) free_results_queue(resources->results_queue);

    // Free file manager
    if (resources->file_manager) free_file_manager(resources->file_manager);
    
    // Free task
    if (resources->task) free_task(resources->task);

    // Free memory allocated for main thread resources
    free(resources);

    // Successfully freed main thread resources
    return SUCCESS;
}

// EOF
