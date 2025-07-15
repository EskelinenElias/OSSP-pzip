#include "../../include/file_manager/unmap_next_file.h"

// Function to unmap a file from memory
int unmap_next_file(file_manager_t* file_manager) {
    
    // Input validation
    if (!file_manager || !file_manager->queue) {
        
        // Invalid input
        fprintf(stderr, "Failed to unmap next file: invalid input\n");
        return ERROR;
    }
    
    // Acquire lock
    if (pthread_mutex_lock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to unmap next file: failed to acquire lock\n"); 
        return ERROR;
    }
    
    // Unmap the next file in the queue
    mapped_file_t* mapped_file = file_manager->queue[file_manager->head]; 
    if (mapped_file) {
        
        // Free mapped file
        if (free_mapped_file(mapped_file) != SUCCESS) {
            
            // Failed to free mapped file
            fprintf(stderr, "Failed to unmap next file: failed to free mapped file\n");
            pthread_mutex_unlock(file_manager->lock);
            return ERROR;
        }
        
        // Reset the spot in the queue 
        file_manager->queue[file_manager->head] = NULL;
    }
    
    // Update queue head index and size
    file_manager->head = (file_manager->head + 1) % file_manager->capacity;
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