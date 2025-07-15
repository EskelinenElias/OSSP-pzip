#include "../../include/file_manager/map_next_file.h"

// Function to map a file into memory
mapped_file_t* map_next_file(file_manager_t* file_manager, const char *filepath) {
    
    // Input validation
    if (!file_manager || !file_manager->queue || !filepath) {
        
        // Invalid input
        fprintf(stderr, "Failed to map next file: invalid input\n");
        return NULL;
    }
    
    // Acquire lock
    if (pthread_mutex_lock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to map next file: failed to acquire lock\n"); 
        return NULL;
    }
        
    // Check that the file manager has room 
    if (file_manager->size == file_manager->capacity) {
        
        // File manager is full
        fprintf(stderr, "Failed to map next file: file manager is full\n");
        return NULL;
    }
    
    // Check that the file exists
    if (access(filepath, F_OK) == -1) {
        
        // File does not exist
        fprintf(stderr, "Failed to map next file: file does not exist\n");
        return NULL;
    }
            
    // Get file stats
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        
        // Failed to get file stats
        fprintf(stderr, "Failed to map next file: failed to get file stats\n"); 
        return NULL;
    }
    
    // Check if the file is already mapped
    mapped_file_t* mapped_file = NULL; 
    for (size_t i = 0; i < file_manager->capacity; i++) {
        
        // Get the next mapped file from the queue
        mapped_file = file_manager->queue[i];
        
        // Get the next pointer from the queue and continue if it is NULL
        if (!mapped_file) continue;
                
        // Compare file stats
        if (mapped_file->st_ino == file_stat.st_ino && mapped_file->st_dev == file_stat.st_dev) {
            
            // Move the mapped file to the tail of the queue
            file_manager->queue[i] = NULL;
            file_manager->queue[file_manager->tail] = mapped_file;
            
            // Update the queue tail index
            file_manager->tail = (file_manager->tail + 1) % file_manager->capacity;
            
            // File is already mapped, break the loop
            break; 
        }
        
        // Reset mapped file 
        mapped_file = NULL; 
    }
    
    // If the file is not already mapped, map it
    if (!mapped_file) {  
        
        // Check if the file is readable
        if (access(filepath, R_OK) == -1) {
            
            // File does not exist or is not readable
            fprintf(stderr, "Failed to map next file: can't access file\n");
            return NULL;
        }
                    
        // Open file and check for errors
        int file; 
        if ((file = open(filepath, 'r')) == -1) {
            
            // Failed to open file
            fprintf(stderr, "Failed to map next file: failed to open file\n");
            return NULL;
        }
        
        // Map file to memory
        mapped_file = init_mapped_file(file, &file_stat); 
        if (!mapped_file) {
            
            // Failed to map next file
            fprintf(stderr, "Failed to map next file: failed to map next file\n");
            close(file);
            return NULL;
        }
        
        // Add the file to the end of the queue
        file_manager->queue[file_manager->tail] = mapped_file; 
        file_manager->tail = (file_manager->tail + 1) % file_manager->capacity; 
        file_manager->size++;
    }
        
    // Release lock
    if (pthread_mutex_unlock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to map next file: failed to release lock\n"); 
        return NULL;
    }
    
    // Successfully mapped new file
    return mapped_file;
}

// EOF 