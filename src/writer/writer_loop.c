#include "../../include/writer/writer_loop.h"

// Writer loop
void* writer_loop(void* args) {
    
    // Cast arguments
    writer_args_t* writer_args = (writer_args_t*)args;
    file_manager_t* file_manager = writer_args->file_manager;
    task_manager_t* task_manager = writer_args->task_manager;
    
    // Free writer arguments
    free(writer_args);
    
    // Input validation 
    if (!file_manager || !task_manager) {
        
        // Failed to create writer thread
        fprintf(stderr, "Writer thread terminated. Invalid input\n");
        return NULL;
    }
    
    // Initialize variables
    result_data_t* current_result_data = NULL; 
    result_data_t* next_result_data = NULL; 
        
    // Start the writer loop
    while ((next_result_data = claim_result_data(task_manager))) {
                
        // Check if the next result is empty
        if (next_result_data->capacity == 0) {
            
            // Unmap the file from memory
            unmap_next_file(file_manager);
            
            // Skip empty result
            if (next_result_data) free_result_data(next_result_data);
            continue;
        }
        
        // Check if the current result can be written to output
        if (current_result_data && current_result_data->capacity > 0) {
                            
            // Handle the boundary between the current result and the next result
            if (handle_boundary(current_result_data, next_result_data) != SUCCESS) {
                
                // Failed to handle boundary
                fprintf(stderr, "Failed to write result to output: error handling boundary\n");
                if (current_result_data) free_result_data(current_result_data);
                if (next_result_data) free_result_data(next_result_data);
                return NULL; 
            } 
                                    
            // Write the current result to output and check for errors
            if (write_encoded_data_to_output(current_result_data) != SUCCESS) {
                
                // Failed to write result to output
                fprintf(stderr, "Failed to write result to output: error writing result to output\n");
                if (current_result_data) free_result_data(current_result_data);
                if (next_result_data) free_result_data(next_result_data);
                return NULL; 
            }       
                  
            // Free the current result        
            if (current_result_data) free_result_data(current_result_data);
            current_result_data = NULL;
        }
    
        // Swap the next result to the current result
        current_result_data = next_result_data;
        next_result_data = NULL; 
    }    
        
    // Check if there is a current result and it has data to write
    if (current_result_data && current_result_data->capacity > 0) {
        
        // Write the last result to output and check for errors
        if (write_encoded_data_to_output(current_result_data) != SUCCESS) {
            
            // Failed to write result to output
            fprintf(stderr, "Error writing result to output\n");
            if (current_result_data) free_result_data(current_result_data);
            if (next_result_data) free_result_data(next_result_data);
            return NULL; 
        }     
        
        // Free the last result
        if (current_result_data) free_result_data(current_result_data);
        current_result_data = NULL;
    }
    
    // Successfully completed writing all results to output stream
    return NULL; 
}

// EOF