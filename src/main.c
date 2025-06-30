#include "../include/main.h"

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