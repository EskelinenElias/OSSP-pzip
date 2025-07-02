#include "../../include/file_manager/free_file_manager.h"

// Function to free file manager
int free_file_manager(file_manager_t* file_manager) {
    
    // Input validation
    if (!file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to free file manager: invalid input\n");
        return ERROR;
    }
    
    // Unmap all files and free memory allocated for mapped file queue
    if (file_manager->mapped_file_queue) {
        
        // Unmap all files
        for (size_t i = 0; i < file_manager->capacity; i++) {
            
            // Get mapped file
            mapped_file_t* mapped_file = file_manager->mapped_file_queue[i]; 
            
            // Unmap file data
            if (munmap(mapped_file->data, mapped_file->size) != SUCCESS) {
                
                // Failed to unmap file
                fprintf(stderr, "Failed to unmap next file: failed to unmap file data from memory\n");
                return ERROR; 
            }
            
            // Close the file
            if (close(mapped_file->file) != SUCCESS) {
                
                // Failed to close file
                fprintf(stderr, "Failed to unmap next file: failed to close file\n");
                return ERROR; 
            }
            
            // Free memory allocated for mapped file
            free(mapped_file);
            mapped_file = NULL; 
            
            // Set pointer to NULL
            file_manager->mapped_file_queue[i] = NULL;
        }
    
        // Free memory allocated for mapped file queue
        free(file_manager->mapped_file_queue);
    }
    
    // Destroy mutex lock 
    if (file_manager->lock) {
        
        // Destroy mutex lock and free memory allocated for it
        pthread_mutex_destroy(file_manager->lock);
        free(file_manager->lock);
    }
    
    // Free memory allocated for file manager
    free(file_manager);
    
    // Successfully freed file manager
    return SUCCESS; 
}

// EOF