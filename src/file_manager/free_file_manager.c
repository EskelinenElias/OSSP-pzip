#include "../../include/file_manager/free_file_manager.h"

// Function to free file manager
int free_file_manager(file_manager_t* file_manager) {
    
    // Input validation
    if (!file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to free file manager: invalid input\n");
        return ERROR;
    }
    
    // Destroy mutex lock 
    if (file_manager->lock && pthread_mutex_destroy(file_manager->lock) != 0) {;
        
        // Failed to destroy mutex lock
        fprintf(stderr, "Failed to free file manager: failed to destroy mutex lock \n"); 
        return ERROR; 
    }
    
    // Free memory allocated for mutex lock
    free(file_manager->lock);
    
    // Free any remaining mapped files in mapped file queue and free memory allocated for mapped file queue
    if (file_manager->queue) {
        
        // Unmap all files
        for (size_t i = 0; i < file_manager->capacity; i++) {
            
            // Get mapped file
            mapped_file_t* mapped_file = file_manager->queue[i];
            if (!mapped_file) continue;
            
            // Free mapped file
            if (free_mapped_file(mapped_file) != SUCCESS) {
                
                // Failed to free mapped file
                fprintf(stderr, "Failed to free file manager: failed to free mapped file\n");
                return ERROR; 
            }
        }
    
        // Free memory allocated for mapped file queue
        free(file_manager->queue);
    }
    
    // Free memory allocated for file manager
    free(file_manager);
    
    // Successfully freed file manager
    return SUCCESS; 
}

// EOF