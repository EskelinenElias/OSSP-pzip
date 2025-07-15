#include "../../include/writer/process_results.h"

// Writer loop
void* process_results(void* args) {
        
    // Input validation 
    if (!args) {
        
        // Invalid input
        fprintf(stderr, "Failed to process results: invalid input\n");
        free(args); 
        return NULL;
    }
    
    // Parse arguments
    writer_thread_args_t* writer_args = (writer_thread_args_t*)args;
    if (!writer_args->results_queue || !writer_args->file_manager || !writer_args->writer || !writer_args->writer->resources) {
        
        // Failed to parse argument
        fprintf(stderr, "Failed to process results: missing required arguments\n");
        free(writer_args); 
        return NULL;
    }
    results_queue_t* results_queue = writer_args->results_queue;
    file_manager_t* file_manager = writer_args->file_manager;
    writer_thread_t* writer = writer_args->writer; 
    writer_thread_resources_t* resources = writer->resources;
    free(writer_args); 
        
    // Start the writer loop
    while ((resources->next_result = claim_result(results_queue))) {
        
        // Check if the next result is empty
        if (resources->next_result->capacity == 0) {
                        
            // Unmap the file from memory
            unmap_next_file(file_manager);
            
            // Free the empty result
            free_result(resources->next_result);
            resources->next_result = NULL; 
            
            // Continue to the next iteration
            continue;
        }
        
        // Check if the current result can be written to output
        if (resources->current_result && resources->current_result->capacity > 0) {
                            
            // Handle the boundary between the current result and the next result
            if (handle_boundary(resources->current_result, resources->next_result) != SUCCESS) {
                
                // Failed to handle boundary
                fprintf(stderr, "Failed to write result to output: error handling boundary\n");
                free_writer_thread_resources(resources); 
                writer->resources = NULL; 
                return NULL; 
            } 
                                                
            // Write the current result to output and check for errors
            if (write_encoded_data_to_output(resources->current_result) != SUCCESS) {
                
                // Failed to write result to output
                fprintf(stderr, "Failed to write result to output: error writing result to output\n");
                free_writer_thread_resources(resources); 
                writer->resources = NULL; 
                return NULL; 
            }       
                  
            // Free the current result        
            free_result(resources->current_result);
            resources->current_result = NULL;
        }
    
        // Swap the next result to the current result
        resources->current_result = resources->next_result;
        resources->next_result = NULL; 
    }    
        
    // Check if there is a current result and it has data to write
    if (resources->current_result && resources->current_result->capacity > 0) {
        
        // Write the last result to output and check for errors
        if (write_encoded_data_to_output(resources->current_result) != SUCCESS) {
            
            // Failed to write result to output
            fprintf(stderr, "Error writing result to output\n");
            free_writer_thread_resources(resources); 
            writer->resources = NULL;  
            return NULL; 
        }     
        
        // Free the last result
        free_result(resources->current_result);
        resources->current_result = NULL;
    }
    
    // Free writer resources
        
    // Successfully completed writing all results to output stream
    return NULL; 
}

// EOF