#include "../include/main.h"

// Main function 
int main(int argc, char *argv[]) {

    // Input validation
    if (argc < 2) {
        
        // Invalid input, print instructions
        printf("pzip: file1 [file2 ...]\n");
        return ERROR;
    }
    
    // Initialize main thread resources
    size_t num_files = argc - 1; 
    main_thread_resources_t* resources = init_main_thread_resources(num_files); 
    if (!resources) {
        
        // Failed to initialize main thread resources
        fprintf(stderr, "Error: failed to initialize main thread resources\n");
        return ERROR; 
    }
                    
    // Encode the files and write encoded data to output
    for (size_t i = 0; i < num_files; i++) {
        
        // Map a new file to memory
        mapped_file_t* current_file = map_next_file(resources->file_manager, argv[i + 1]); 
        if (!current_file) {
            
            // Failed to map file 
            free_main_thread_resources(resources); 
            fprintf(stderr, "Error: failed to map file '%s'\n", argv[i + 1]);
            return ERROR; 
        }
                                
        // Yield task to task manager until end of file is reached
        for (size_t t = 0; t < current_file->size; t += TASK_SIZE) {
            
            // Get pointer to task and task size 
            char* data = current_file->data + t; 
            size_t size = fmin(TASK_SIZE, current_file->size - t); 
            
            // Yield task to task queue
            if (yield_task(resources->task_queue, data, size) != SUCCESS) {
                
                // Failed to yield task to tasks queue
                fprintf(stderr, "Error: failed to yield task to tasks queue\n");
                free_main_thread_resources(resources);
                return ERROR; 
            }
        }
                                            
        // Yield EOF task to tasks queue
        if (yield_task(resources->task_queue, NULL, 0) != SUCCESS) {
            
            // Failed to yield EOF task to tasks queue
            fprintf(stderr, "Error: failed to yield EOF task to tasks queue\n");
            free_main_thread_resources(resources);
            return ERROR; 
        }
    }
        
    // Cleanup routine
    if (free_main_thread_resources(resources) != SUCCESS) {
        
        // Failed to clean up process
        fprintf(stderr, "Error: failed to free resources\n");
        return ERROR; 
    };
    
    // Successfully encoded data
    return SUCCESS;
}

// EOF