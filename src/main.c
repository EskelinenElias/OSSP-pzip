#include "../include/main.h"
#include <stdio.h>

// Function to initialize process variables
process_vars_t* init_process(size_t num_cores, size_t num_files) {
        
    // Input validation
    if (num_cores < 1 || num_files < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize process, invalid input\n"); 
        return NULL;
    }
    
    // Allocate memory for process variables and check for errors
    process_vars_t* process = (process_vars_t*)malloc(sizeof(process_vars_t));
    if (!process) {
        
        // Failed to allocate memory for process
        fprintf(stderr, "Failed to initialize process, failed to allocate memory\n");
        return NULL;
    }
    
    // Initialize process fields
    process->file_manager = NULL; 
    process->task_manager = NULL;
    process->workers = NULL; 
    process->num_workers = num_cores - 1 ; 
    process->writer = NULL;
    
    // Initialize file manager and check for errors
    if (!(process->file_manager = init_file_manager(num_files))) {
        
        // Failed to initialize file manager
        fprintf(stderr, "Failed to initialize process: failed to initialize file manager\n");
        return free_process(process); ; 
    }
    
    // Initialize a task task_manager and check for errors
    if (!(process->task_manager = init_task_manager(MAX_CAPACITY))) {
        
        // Failed to initialize task task_manager
        fprintf(stderr, "Failed to initialize process: failed to initialize task manager\n");
        return free_process(process); ; 
    }
    
    if (!process->task_manager->lock) {
        
        fprintf(stderr, "Failed to initialize process: failed to initialize task manager lock\n");
        return free_process(process); 
    }
    
    // Allocate memory for workers and check for errors
    if (!(process->workers = (pthread_t**)malloc(sizeof(pthread_t*) * process->num_workers))) {
        
        // Failed to allocate memory for workers
        fprintf(stderr, "Failed to initialize process: failed to allocate memory for workers\n");
        return free_process(process); ; 
    }
    
    // Initialize workers and check for errors
    for (size_t i = 0; i < process->num_workers; i++) {
        
        // Initialize a worker and check for errors
        if (!(process->workers[i] = init_worker(process->task_manager))) {
            
            // Failed to initialize worker
            fprintf(stderr, "Failed to initialize process: failed to initialize worker %zu\n", i);
            return free_process(process); ; 
        }
    }
    
    // Initialize writer and check for errors
    if (!(process->writer = init_writer(process->file_manager, process->task_manager))) {
        
        // Failed to initialize writer
        fprintf(stderr, "Failed to initialize process: failed to initialize writer\n");
        return NULL; 
    }
        
    // Successfully initialized process
    return process;
}

// Function to free process variables
process_vars_t* free_process(process_vars_t* process) {
        
    // Send termination signals
    for (size_t i = 0; i < process->num_workers; i++) {
        send_termination_signal(process->task_manager);
    }
        
    // Wait for all threads to complete
    if (process->workers) {
        for (size_t i = 0; i < process->num_workers; i++) {
            
            // Check if the worker still exists
            if (process->workers[i]) {
                
                // Wait for worker to complete
                pthread_join(*process->workers[i], NULL);
                
                // Set the worker to NULL
                process->workers[i] = NULL;
            }
        }
    }
    
    // Wait for writer to complete
    if (process->writer) pthread_join(*process->writer, NULL);
    
    // Free task manager
    if (process->task_manager) free_manager(process->task_manager);
    
    // Free file manager
    if (process->file_manager) free_file_manager(process->file_manager);
            
    // Free process variables
    free(process);
    return NULL; 
}

// Main function 
int main(int argc, char *argv[]) {

    // Input validation
    if (argc < 2) {
        
        // Invalid input, print instructions
        printf("pzip: file1 [file2 ...]\n");
        return ERROR;
    }
    
    // Initialize process variables
    size_t num_cores = get_num_cores();
    size_t num_files = argc - 1; 
    process_vars_t* process = init_process(num_cores, num_files); 
    if (!process) return ERROR; 
                
    // Encode the files and write encoded data to output
    for (size_t i = 0; i < num_files; i++) {
        
        // Map a new file to memory
        mapped_file_t* current_file = map_next_file(process->file_manager, argv[i + 1]); 
        if (!current_file) {
            
            // Failed to map file 
            fprintf(stderr, "Failed to map file to memory\n"); 
            free_process(process); 
            return ERROR; 
        }
                                
        // Yield tasks to task task_manager until end of file is reached
        for (size_t t = 0; t < current_file->size; t += TASK_SIZE) {
            
            // Determine task data and size 
            char* task_data = current_file->data + t; 
            size_t task_size = fmin(TASK_SIZE, current_file->size - t); 
            
            // Yield task to task task_manager
            if (yield_task_data(process->task_manager, task_data, task_size) != SUCCESS) {
                
                // Failed to yield task to task task_manager
                fprintf(stderr, "Failed to yield task to task task_manager\n");
                free_process(process);
                return ERROR; 
            }
        }
                                        
        // Yield an end of file task to task task_manager
        if (yield_task_data(process->task_manager, NULL, 0) != SUCCESS) {
            
            // Failed to yield EOF task to task task_manager
            fprintf(stderr, "Failed to yield task to task task_manager\n");
            free_process(process);
            return ERROR; 
        }
    }
        
    // Cleanup routine
    free_process(process);
    
    // Successfully encoded data
    return SUCCESS;
}