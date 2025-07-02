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
    if (!process) {
        
        // Failed to initialize process variables
        return ERROR; 
    }
                    
    // Encode the files and write encoded data to output
    for (size_t i = 0; i < num_files; i++) {
        
        // Map a new file to memory
        mapped_file_t* current_file = map_next_file(process->file_manager, argv[i + 1]); 
        if (!current_file) {
            
            // Failed to map file 
            free_process(process); 
            return ERROR; 
        }
                                
        // Yield task to task manager until end of file is reached
        for (size_t t = 0; t < current_file->size; t += TASK_SIZE) {
            
            // Get pointer to task and task size 
            char* data = current_file->data + t; 
            size_t size = fmin(TASK_SIZE, current_file->size - t); 
            
            // Initialize task
            task_t* task = init_task(data, size, process->results_queue);
            if (!task) {
                
                // Failed to initialize task
                free_process(process);
                return ERROR;
            }
            
            // Yield task to task manager
            if (yield_task(process->tasks_queue, task) != SUCCESS) {
                
                // Failed to yield task to task manager
                free_task(task); 
                free_process(process);
                return ERROR; 
            }
        }
                                        
        // Yield EOF task to task manager (writer thread will tell file manager to unmap and close the next file in queue when it encounters this)
        if (yield_task(process->tasks_queue, NULL) != SUCCESS) {
            
            // Failed to yield EOF task to task task_manager
            free_process(process);
            return ERROR; 
        }
    }
    
    // Cleanup routine
    if (free_process(process) != SUCCESS) {
        
        // Failed to clean up process
        return ERROR; 
    };
    
    // Successfully encoded data
    return SUCCESS;
}

// EOF