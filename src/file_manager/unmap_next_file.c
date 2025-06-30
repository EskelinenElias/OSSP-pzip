#include "../../include/file_manager/unmap_next_file.h"

// Function to unmap a file from memory
int unmap_next_file(file_manager_t* file_manager) {
    
    // Input validation
    if (!file_manager || !file_manager->mapped_file_queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to unmap next file: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock
    if (pthread_mutex_lock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to map file: failed to acquire lock\n"); 
        return ERROR;
    }
    
    // Unmap the next file in the queue
    mapped_file_t* mapped_file = file_manager->mapped_file_queue[file_manager->front]; 
    if (mapped_file) {
        
        // Unmap the file from memory
        if (munmap(mapped_file->data, mapped_file->size) != SUCCESS) {
            
            // Failed to unmap file
            fprintf(stderr, "Failed to unmap next file: failed to unmap file data from memory\n");
            close(mapped_file->file);
            free(mapped_file); 
            return ERROR; 
        }
        
        // Close the file
        if (close(mapped_file->file) != SUCCESS) {
            
            // Failed to close file
            fprintf(stderr, "Failed to unmap next file: failed to close file\n");
            free(mapped_file); 
            return ERROR; 
        }
        
        // Free the mapped file structure
        free(mapped_file);
        mapped_file = NULL; 
    }
    
    // Update the front index of the queue
    file_manager->mapped_file_queue[file_manager->front] = NULL;
    file_manager->front = (file_manager->front + 1) % file_manager->capacity;
    file_manager->size--; 
    
    // Release lock
    if (pthread_mutex_unlock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to map file: failed to release lock\n"); 
        return ERROR;
    }
    
    // Successfully unmapped file
    return SUCCESS;
}

// EOF