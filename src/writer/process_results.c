#include "../../include/writer/process_results.h"

// Writer loop
void* process_results(void* args) {
    
    // Cast arguments
    writer_args_t* writer_args = (writer_args_t*)args;
    file_manager_t* file_manager = writer_args->file_manager;
    results_queue_t* results_queue = writer_args->results_queue;
    
    // Free writer arguments
    free(writer_args);
    
    // Input validation 
    if (!file_manager || !results_queue) {
        
        // Failed to create writer thread
        fprintf(stderr, "Writer thread terminated. Invalid input\n");
        return NULL;
    }
    
    // Initialize variables
    result_t* current_result = NULL; 
    result_t* next_result = NULL; 
        
    // Start the writer loop
    while ((next_result = claim_result(results_queue))) {
        
        // Check if the next result is empty
        if (next_result->capacity == 0) {
                        
            // Unmap the file from memory
            unmap_next_file(file_manager);
            
            // Skip empty result
            if (next_result) free_result(next_result);
            continue;
        }
        
        // Check if the current result can be written to output
        if (current_result && current_result->capacity > 0) {
                            
            // Handle the boundary between the current result and the next result
            if (handle_boundary(current_result, next_result) != SUCCESS) {
                
                // Failed to handle boundary
                fprintf(stderr, "Failed to write result to output: error handling boundary\n");
                if (current_result) free_result(current_result);
                if (next_result) free_result(next_result);
                return NULL; 
            } 
                                                
            // Write the current result to output and check for errors
            if (write_encoded_data_to_output(current_result) != SUCCESS) {
                
                // Failed to write result to output
                fprintf(stderr, "Failed to write result to output: error writing result to output\n");
                if (current_result) free_result(current_result);
                if (next_result) free_result(next_result);
                return NULL; 
            }       
                  
            // Free the current result        
            if (current_result) free_result(current_result);
            current_result = NULL;
        }
    
        // Swap the next result to the current result
        current_result = next_result;
        next_result = NULL; 
    }    
        
    // Check if there is a current result and it has data to write
    if (current_result && current_result->capacity > 0) {
        
        // Write the last result to output and check for errors
        if (write_encoded_data_to_output(current_result) != SUCCESS) {
            
            // Failed to write result to output
            fprintf(stderr, "Error writing result to output\n");
            if (current_result) free_result(current_result);
            if (next_result) free_result(next_result);
            return NULL; 
        }     
        
        // Free the last result
        if (current_result) free_result(current_result);
        current_result = NULL;
    }
    
    // Successfully completed writing all results to output stream
    return NULL; 
}

// EOF