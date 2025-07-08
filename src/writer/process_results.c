#include "../../include/writer/process_results.h"

// Function to initialize writer resources
writer_resources_t* init_writer_resources() {
    
    // Allocate memory for writer arguments
    writer_resources_t* resources = calloc(1, sizeof(writer_resources_t));
    if (!resources) {
        
        // Failed to allocate memory for writer arguments
        fprintf(stderr, "Failed to initialize writer resources: failed to allocate memory for writer arguments\n");
        return NULL;
    }
    
    // Successfully allocated memory for writer arguments
    return resources;
}

// Function to free writer resources
int free_writer_resources(writer_resources_t* resources) {
    
    // Input validation
    if (!resources) {
        
        // Invalid input
        fprintf(stderr, "Failed to free writer resources: invalid input\n");
        return ERROR;
    }
    
    // Free memory allocated for writer resources
    if (resources->current_result) free_result(resources->current_result); 
    if (resources->next_result) free_result(resources->next_result);
    
    // Free memory allocated for writer resources structure
    free(resources);
    
    // Successfully freed memory for writer resources
    return SUCCESS;
}

// Writer loop
void* process_results(void* args) {
        
    // Input validation 
    if (!args) {
        
        // Invalid input
        fprintf(stderr, "Failed to process results: invalid input\n");
        return NULL;
    }
    
    // Parse arguments
    writer_args_t* writer_args = (writer_args_t*)args;
    if (!writer_args->results_queue || !writer_args->file_manager) {
        
        // Failed to parse argument
        fprintf(stderr, "Failed to process results: missing required arguments\n");
        return NULL;
    }
    results_queue_t* results_queue = writer_args->results_queue;
    file_manager_t* file_manager = writer_args->file_manager;
    free(writer_args); 
    
    // Initialize writer resources
    writer_resources_t* resources = init_writer_resources(); 
    if (!resources) {
        
        // Failed to initialize writer resources
        fprintf(stderr, "Failed to process results: failed to initialize writer resources\n");
        return NULL;
    }
        
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
                free_writer_resources(resources); 
                return NULL; 
            } 
                                                
            // Write the current result to output and check for errors
            if (write_encoded_data_to_output(resources->current_result) != SUCCESS) {
                
                // Failed to write result to output
                fprintf(stderr, "Failed to write result to output: error writing result to output\n");
                free_writer_resources(resources); 
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
            free_writer_resources(resources); 
            return NULL; 
        }     
        
        // Free the last result
        free_result(resources->current_result);
        resources->current_result = NULL;
    }
    
    // Free writer resources
    free_writer_resources(resources);
        
    // Successfully completed writing all results to output stream
    return NULL; 
}

// EOF