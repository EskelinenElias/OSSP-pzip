#include "../../include/file_manager/free_file_manager.h"

// Function to free file manager
void* free_file_manager(file_manager_t* file_manager) {
    
    // Input validation
    if (!file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to free file manager: invalid input\n");
        return NULL;
    }
    
    // Unmap all files and free memory allocated for mapped file queue
    if (file_manager->mapped_file_queue) {
        
        // Unmap all files
        for (size_t i = 0; i < file_manager->capacity; i++) unmap_next_file(file_manager); 
    
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
    return NULL; 
}

// EOF