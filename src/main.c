#include "../include/main.h"

// Function to initialize process variables
process_vars_t* init_process(size_t num_workers, size_t num_files) {
        
    // Input validation
    if (num_workers < 1 || num_files < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize process, invalid input"); 
        return NULL;
    }
    
    // Allocate memory for process variables and check for errors
    process_vars_t* process = malloc(sizeof(process_vars_t));
    if (!process) {
        
        // Failed to allocate memory for process
        fprintf(stderr, "Failed to initialize process, failed to allocate memory");
        return NULL;
    }
    
    // Initialize process fields
    process->file_manager = NULL; 
    process->task_manager = NULL;
    process->workers = NULL; 
    process->num_workers = num_workers; 
    process->writer = NULL;
    
    // Initialize file manager and check for errors
    if (!(process->file_manager = init_file_manager(num_files))) {
        
        // Failed to initialize file manager
        fprintf(stderr, "Failed to initialize file manager");
        return NULL; 
    }
    
    // Initialize a task task_manager and check for errors
    if (!(process->task_manager = init_manager(MAX_CAPACITY))) {
        
        // Failed to initialize task task_manager
         fprintf(stderr, "Failed to initialize task task_manager");
        return NULL; 
    }
    
    // Initialize workers and check for errors
    if (!(process->workers = init_workers(process->num_workers, process->task_manager))) { 
        
        // Failed to initialize workers
        fprintf(stderr, "Failed to initialize workers"); 
        return NULL; 
    }
    
    // Initialize writer and check for errors
    if (!(process->writer = init_writer(process->file_manager, process->task_manager))) {
        
        // Failed to initialize writer
        fprintf(stderr, "Failed to initialize writer");
        return NULL; 
    }
        
    // Successfully initialized process
    return process;
}

// Function to free process variables
process_vars_t* free_process(process_vars_t* process) {
        
    // Terminate workers 
    if (process->workers && process->num_workers > 0) {
        terminate_workers(process->workers, process->num_workers, process->task_manager);
    }
    
    // Terminate writer
    if (process->writer) terminate_writer(process->writer);
            
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
        
    // Encode the files and write encoded data to output
    for (size_t i = 0; i < num_files; i++) {
                
        // Map a new file to memory
        mapped_file_t* current_file = map_next_file(process->file_manager, argv[i + 1]); 
        if (!current_file) {
            
            // Failed to map file 
            fprintf(stderr, "Failed to map file to memory"); 
            free_process(process); 
            return ERROR; 
        }
                
        // Yield tasks to task task_manager until end of file is reached
        for (size_t t = 0; t < current_file->size; t += TASK_SIZE) {
            
            // Determine task data and size 
            char* task_data = current_file->data + t; 
            size_t task_size = fmin(TASK_SIZE, current_file->size - t); 
            
            // Yield task to task task_manager
            if (yield_task(process->task_manager, task_data, task_size) != SUCCESS) {
                
                // Failed to yield task to task task_manager
                fprintf(stderr, "Failed to yield task to task task_manager\n");
                free_process(process);
                return ERROR; 
            }
        }
                                
        // Yield an end of file task to task task_manager
        if (yield_task(process->task_manager, NULL, 0) != SUCCESS) {
            
            // Failed to yield EOF task to task task_manager
            fprintf(stderr, "Failed to yield task to task task_manager\n");
            free_process(process);
            return ERROR; 
        }
    }
        
    // Wait for all tasks to complete
    wait_for_completion(process->task_manager); 
    
    // Cleanup routine
    free_process(process);
    
    // Successfully encoded data
    return SUCCESS;
}