#include "../../include/file_manager/free_mapped_file.h"

// Function to free mapped file (a memory mapped file)
int free_mapped_file(mapped_file_t* mapped_file) {
    
    // Input validation
    if (!mapped_file) {
        
        // Invalid input
        fprintf(stderr, "Failed to free mapped file: invalid input\n");
        return ERROR;
    }
    
    // Unmap the file from memory
    if (mapped_file->data && munmap(mapped_file->data, mapped_file->size) != 0) {
        
        // Failed to unmap file
        fprintf(stderr, "Failed to free mapped file: failed to unmap file data from memory\n");
        close(mapped_file->file);
        free(mapped_file); 
        return ERROR; 
    }
    
    // Close the file
    if (mapped_file->file != -1 && close(mapped_file->file) != 0) {
        
        // Failed to close file
        fprintf(stderr, "Failed to free mapped file: failed to close file\n");
        free(mapped_file); 
        return ERROR; 
    }
    
    // Free the mapped file structure
    free(mapped_file);
    
    // Successfully freed mapped file
    return SUCCESS;
}

// EOF