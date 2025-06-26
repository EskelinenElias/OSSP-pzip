#include "../../include/worker/do_work.h"

// Thread worker function
void* do_work(void* args) {
    
    // Cast arguments
    worker_args_t* worker_args = (worker_args_t*) args;
    task_manager_t *task_manager = worker_args->task_manager;
            
    // Process tasks until a termination task is acquired
    task_t* task = NULL; 
    while ((task = claim_task(task_manager)) != NULL) {
            
        // Process the current task 
        result_data_t* result_data = NULL;
        if (!(result_data = encode(&task->task_data))) {
            
            // Terminate the thread 
            fprintf(stderr, "Failed to complete task: error occurred during encoding"); 
            return NULL; 
        }; 
                        
        // Yield the result to the task_manager
        if (yield_result(task_manager, task->result, result_data) != SUCCESS) {
            
            // Failed to yield result to task_manager
            fprintf(stderr, "Failed to complete task: failed to yield result\n"); 
            if (result_data) free_result_data(result_data); 
            return NULL; 
        };
        
        // Free the memory allocated for the task 
        if (free_task(task) != SUCCESS) {
            
            // Failed to free task
            fprintf(stderr, "Failed to complete task: failed to free task\n");
            return NULL;
        }
    }
    
    // Yield a NULL result to the task_manager as a sign of termination
    // if (yield_result(task_manager, task->result, NULL) != SUCCESS) {
        
    //     // Failed to yield result to task_manager
    //     fprintf(stderr, "Failed to complete task: failed to yield NULL result\n"); 
    //     return NULL; 
    // };
    
    // Successfully completed tasks
    return NULL; 
}

// Function to initialize worker threads
// pthread_t* init_workers(size_t num_workers, task_manager_t *task_manager) {
    
//     // Allocate memory for worker threads and check for errors
//     pthread_t* workers = (pthread_t*)malloc(num_workers * sizeof(pthread_t));
//     if (!workers) {
        
//         // Failed to allocate memory for worker threads
//         fprintf(stderr, "Failed to allocate memory for workers");
//         return NULL;
//     }
    
//     // Initialize worker threads
//     for (int i = 0; i < num_workers; i++) { 
                
//         // Start a new thread and check for errors
//         if (pthread_create(&workers[i], NULL, thread_worker, task_manager) != 0) {
                        
//             // Failed to initialize worker
//             fprintf(stderr, "Failed to initialize workers");
//             terminate_workers(workers, num_workers, task_manager); 
//             return NULL;
//         }
//     }
    
//     // Successfully initialized all worker threads
//     return workers;
// }

// // Function to terminate all worker threads
// // int terminate_workers(pthread_t *threads, size_t num_workers, task_manager_t *task_manager) {
    
// //     // Input validation
//     if (!threads || !task_manager) {
        
//         // Invalid input
//         fprintf(stderr, "Invalid input");
//         return ERROR;
//     };
    
//     // Send termination signal to all threads
//     for (int i = 0; i < num_workers; i++) {
        
//         // Yield a termination task to one worker
//         if (yield_termination_task(task_manager) != SUCCESS) {
            
//             // Failed to yield termination task 
//             fprintf(stderr, "Failed to yield termination task");
            
//             // Force termination 
//             // force_termination(task_manager); 
//             break; 
//         }
//     }
    
//     // Wait until all threads have terminated
//     for (int i = 0; i < num_workers; i++) {
        
//         // Wait for thread to return
//         pthread_join(threads[i], NULL);
//     }
    
//     return SUCCESS;
// }
