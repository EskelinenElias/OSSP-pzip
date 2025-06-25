#include "../include/writer.h"

// Function to initialize writer thread
pthread_t* init_writer(file_manager_t* file_manager, task_manager_t* task_manager) {
    
    // Allocate memory for writer thread and check for errors
    pthread_t* writer_thread = malloc(sizeof(pthread_t));
    if (!writer_thread) {
        
        // Failed to allocate memory for writer thread
        fprintf(stderr, "Failed to allocate memory for writer thread\n");
        return NULL;
    }
    
    // Allocate memory for writer arguments
    writer_args_t* writer_args = malloc(sizeof(writer_args_t));
    if (!writer_args) {
        
        // Failed to allocate memory for writer arguments
        fprintf(stderr, "Failed to allocate memory for writer arguments\n");
        free(writer_thread);
        return NULL;
    }
    
    // Initialize writer arguments
    writer_args->file_manager = file_manager;
    writer_args->task_manager = task_manager;
    
    // Initialize writer thread
    if (pthread_create(writer_thread, NULL, writer, writer_args) != 0) {
        
        // Failed to create writer thread
        fprintf(stderr, "Failed to create writer thread\n");
        free(writer_thread);
        free(writer_args);
        return NULL;
    }
    
    // Successfully initialized writer thread
    return writer_thread;
}

// Writer loop
void* writer(void* args) {
    
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
    result_data_t* current_result = NULL; 
    result_data_t* next_result = NULL; 
        
    // Start the writer loop
    while ((next_result = claim_result(task_manager)) != NULL) {
                
        // Check if the next result is empty
        if (next_result->capacity == 0) {
            
            // Unmap the file from memory
            unmap_next_file(file_manager);
            
            // Skip empty result
            free_result(next_result);
            continue;
        }
        
        // Check if the current result can be written to output
        if (current_result && current_result->capacity > 0) {
                            
            // Handle the boundary between the current result and the next result
            handle_boundary(current_result, next_result); 
                                    
            // Write the current result to output and check for errors
            if (write_encoded_data_to_output(current_result) != SUCCESS) {
                
                // Failed to write result to output
                fprintf(stderr, "Error writing result to output\n");
                if (current_result) free_result(current_result);
                if (next_result) free_result(next_result);
                return NULL; 
            }       
                  
            // Free the current result        
            free_result(current_result);
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
        free_result(current_result);
        current_result = NULL;
    }
    
    // Successfully completed writing all results to output stream
    return NULL; 
}

// Function to terminate writer thread
void* terminate_writer(pthread_t* writer) {
    pthread_join(*writer, NULL);
    return NULL; 
}
