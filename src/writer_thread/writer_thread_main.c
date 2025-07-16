#include "../../include/writer_thread/writer_thread_main.h"

// Writer thread loop
void* writer_thread_main(void* args) {
        
    // Input validation
    writer_thread_t* writer = (writer_thread_t*)args;
    if (!writer || !writer->results_queue || !writer->file_manager) {
        
        // Failed to parse argument
        fprintf(stderr, "Failed to process results: invalid input\n");
        return NULL;
    }
    
    // Setup shorthands for results queue and file manager
    results_queue_t* results_queue = writer->results_queue;
    file_manager_t* file_manager = writer->file_manager;
        
    // Start processing results
    while ((writer->next_result = claim_result(results_queue))) {
        
        // Check if the next result is empty
        if (writer->next_result->capacity == 0) {
                        
            // Unmap the file from memory
            unmap_next_file(file_manager);
            
            // Free the empty result
            free_result(writer->next_result);
            writer->next_result = NULL; 
            
            // Continue to the next iteration
            continue;
        }
        
        // Check if the current result can be written to output
        if (writer->current_result && writer->current_result->capacity > 0) {
                            
            // Handle the boundary between the current result and the next result
            if (handle_boundary(writer->current_result, writer->next_result) != SUCCESS) {
                
                // Failed to handle boundary
                fprintf(stderr, "Failed to write result to output: error handling boundary\n");
                if (writer->current_result) free_result(writer->current_result);
                writer->current_result = NULL;
                if (writer->next_result) free_result(writer->next_result);
                writer->next_result = NULL;
                return NULL; 
            } 
                                                
            // Write the current result to output and check for errors
            if (write_encoded_data_to_output(writer->current_result) != SUCCESS) {
                
                // Failed to write result to output
                fprintf(stderr, "Failed to write result to output: error writing result to output\n");
                if (writer->current_result) free_result(writer->current_result);
                writer->current_result = NULL;
                if (writer->next_result) free_result(writer->next_result);
                writer->next_result = NULL;
                return NULL; 
            }       
                  
            // Free the current result        
            free_result(writer->current_result);
            writer->current_result = NULL;
        }
    
        // Swap the next result to the current result
        writer->current_result = writer->next_result;
        writer->next_result = NULL; 
    }    
        
    // Check if there is a current result and it has data to write
    if (writer->current_result && writer->current_result->capacity > 0) {
        
        // Write the last result to output and check for errors
        if (write_encoded_data_to_output(writer->current_result) != SUCCESS) {
            
            // Failed to write result to output
            fprintf(stderr, "Error writing result to output\n");
            if (writer->current_result) free_result(writer->current_result);
            writer->current_result = NULL;
            if (writer->next_result) free_result(writer->next_result);
            writer->next_result = NULL;
            return NULL; 
        }     
        
        // Free the last result
        free_result(writer->current_result);
        writer->current_result = NULL;
    }
    
    // Free writer resources
    if (writer->current_result) free_result(writer->current_result);
    writer->current_result = NULL;
    if (writer->next_result) free_result(writer->next_result);
    writer->next_result = NULL;
    
    // Successfully completed writing all results to output stream
    return NULL; 
}

// EOF