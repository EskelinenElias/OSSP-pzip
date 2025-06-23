#include "../include/main.h"

// Cleanup routine
process_vars_t* free_process(process_vars_t* process) {
        
    // Terminate workers 
    if (process->workers && process->num_workers > 0) {
        terminate_workers(process->workers, process->num_workers, process->manager);
    }
            
    // Free task manager
    if (process->manager) free_manager(process->manager);
    
    // Free results
    if (process->current_result) free_result(process->current_result);
    if (process->next_result) free_result(process->next_result); 
    
    // Unmap files
    for (size_t i = 0; i < process->num_files; i++) {
        
        // Unmap file if necessary
        if (process->files[i]) process->files[i] = unmap_file(process->files[i]); 
    }
    
    // Free memory allocated for file pointers
    free(process->files); 
        
    // Set pointers to NULL to avoid dangling pointers
    process->manager = NULL;
    process->workers = NULL; 
    process->num_workers = 0;
    process->current_result = NULL;
    process->next_result = NULL;
    process->files = NULL; 
    process->num_files = 0; 
    
    // Free process variables
    free(process);
    return NULL; 
}

int panic(process_vars_t* process, char* message, int error_code) {
    
    // Print error message
    fprintf(stderr, "%s\n", message);
    
    // Perform cleanup routine
    process = free_process(process);
    
    // Return error code
    return error_code;
}

// Function to initialize the process
process_vars_t* init_process(size_t num_workers, size_t num_files) {
        
    // Input validation
    if (num_workers < 1 || num_files < 1) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize process, invalid input"); 
        return NULL;
    }
    
    // Allocate memory for process variables
    process_vars_t* process = malloc(sizeof(process_vars_t));
    
    // Initialize process fields
    process->manager = NULL;
    process->workers = NULL; 
    process->num_workers = num_workers;
    process->current_result = NULL;
    process->next_result = NULL;
    process->files = NULL; 
    process->num_files = num_files; 
    
    // Initialize a task manager and check for errors
    if (!(process->manager = init_manager(MAX_CAPACITY))) {
        
        // Failed to initialize task manager
         fprintf(stderr, "Failed to initialize task manager");
        return NULL; 
    }
    
    // Initialize workers and check for errors
    if (!(init_workers(process->num_workers, process->manager))) { 
        
        // Failed to initialize workers
        fprintf(stderr, "Failed to initialize workers"); 
        return NULL; 
    }
    
    // Allocate memory for file pointers
    if (!(process->files = malloc(sizeof(mapped_file_t*) * num_files))) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to allocate memory for file pointers"); 
        return NULL; 
    }        
        
    // Successfully initialized process
    return process;
}

// Function to map a file to memory
mapped_file_t* map_next_file(process_vars_t* process, char* path, size_t index) {
    process->files[index] = map_file(path);
    return process->files[index];
}

// Function to write results 
int write_results(process_vars_t* process) {
    
    // Iterate over the processed chunks to handle boundaries and write to output stream
    while ((process->next_result = claim_result(process->manager))) {
                    
        // Check if the next result is empty
        if (process->next_result && process->next_result->capacity == 0) {
            
            // Unmap the next mapped file
            for (int i = 0; i < process->num_files; i++) {
                
                // Check if the file is mapped
                if (process->files[i]) {
                    
                    // Unmap the file
                    process->files[i] = unmap_file(process->files[i]);
                    break; 
                }
            }
            
            // Free the empty result
            free_result(process->next_result);
            process->next_result = NULL;
            
            // Move to the start of the loop (claim new result)
            continue; 
        }
        
        // Check if the current result can be written to output
        if (process->current_result) {
                            
            // Handle the boundary between the current task and the next task
            handle_boundary(process->current_result, process->next_result); 
                                    
            // Write the encoded data to the output stream and check for errors
            if (write_encoded_data_to_output(process->current_result) != SUCCESS) {
                
                // Error; perform cleanup routine 
                fprintf(stderr, "Error writing encoded data to output stream\n");
                free_process(process); 
                return ERROR;
            }       
                  
            // Free the current result        
            free_result(process->current_result);
            process->current_result = NULL;
        }
                    
        // Swap the next result to the current result
        process->current_result = process->next_result;
        process->next_result = NULL; 
    }
    
    // Successfully wrote available results
    return SUCCESS; 
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
    process_vars_t* process = init_process(get_num_cores(), argc - 1); 
    
    // Open first file and start encoding loop
    size_t file_index = 0;
    size_t task_index = 0; 
    size_t unmap_index = 0; 
    mapped_file_t* current_file = NULL; 
        
    // Encode the files and write encoded data to output
    while (TRUE) {
        
        // Check if a new file needs to be mapped
        if (!current_file && file_index < argc - 1) {
            
            // Map a new file to memory
            if (!(process->files[file_index] = map_file(argv[file_index + 1]))) {
                
                // Failed to map file 
                fprintf(stderr, "Failed to map file to memory"); 
                free_process(process); 
                return ERROR; 
            }
            
            // File mapped to memory, reset task index
            task_index = 0; 
            
            // Initialize current file pointer
            current_file = process->files[file_index];
            
            // Increment file index
            file_index++; 
        }
                
        if (current_file) {

            // Check if there is still unprocessed data in the file
            while (task_index < current_file->size) {
                            
                // Determine task size 
                size_t size_left = current_file->size - task_index; 
                size_t task_size = fmin(TASK_SIZE, size_left); 
                
                // Task data 
                char* task_data = current_file->data + task_index; 
                
                // Yield the task to the task manager, if there is room
                if (yield_task(process->manager, task_data, task_size) != SUCCESS) break; 
                    
                // Increment task index
                task_index += TASK_SIZE; 
            } 
                
            // Check if the end of the file has been reached
            if (task_index >= current_file->size) {
            
                // Submit an empty task to indicate end of file
                if (yield_task(process->manager, NULL, 0) == SUCCESS) {
                    
                    // Increment file index and move to the start of the loop to map a new file
                    current_file = NULL; 
                    continue; 
                }
            }
        }
        
        // Write available results to output
        if (write_results(process) == SUCCESS) {
            
            // Failed to write available results to output
            perror("Failed to write available results to output");
            free_process(process);
            return ERROR;
        }
            
        if (!current_file) break; 
    }
    
    // Write the last results to output
    if (write_results(process) == SUCCESS) {
        
        // Failed to write available results to output
        perror("Failed to write available results to output");
        free_process(process);
        return ERROR;
    }    
    
    // Free the last result
    free_result(process->current_result);
    process->current_result = NULL;
    
    // Cleanup routine
    free_process(process);
    
    // Successfully encoded data
    return SUCCESS;
}