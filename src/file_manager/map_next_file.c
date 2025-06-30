#include "../../include/file_manager/map_next_file.h"

// Function to map a file into memory
mapped_file_t* map_next_file(file_manager_t* file_manager, const char *filepath) {
    
    // Input validation
    if (!filepath || !file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to map file: invalid input\n");
        return NULL;
    }
    
    // Acquire lock
    if (pthread_mutex_lock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to map file: failed to acquire lock\n"); 
        return NULL;
    }
        
    // Check that the file manager has room 
    if (file_manager->size == file_manager->capacity) {
        
        // File manager is full
        fprintf(stderr, "Failed to map file: file manager is full\n");
        return NULL;
    }
            
    // Get file stats and check for errors
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        
        // Failed to get file stats
        fprintf(stderr, "Failed to map file: failed to get file stats\n"); 
        return NULL;
    }
    
    // Check if the file's stats match the stats of any of the already mapped files
    mapped_file_t* mapped_file = NULL; 
    for (size_t i = 0; i < file_manager->capacity; i++) {
        
        // Get the next pointer from the queue and continue if it is NULL
        if (!(mapped_file = file_manager->mapped_file_queue[i])) continue;
                
        // Compare file stats
        if (mapped_file->st_ino == file_stat.st_ino && mapped_file->st_dev == file_stat.st_dev) {
            
            // Remove the mapped file from it's current spot in the queue (it will be added back to the end of the queue later)
            file_manager->mapped_file_queue[i] = NULL;
            break; 
        }
    }
    
    // Check if a file with matching stats was found (that is, the file has already been mapped)
    if (!mapped_file) {
        
        // Check if the file exists and is readable
        if (access(filepath, F_OK) == -1 || access(filepath, R_OK) == -1) {
            
            // File does not exist or is not readable
            fprintf(stderr, "Failed to map file: can't access file\n");
            return NULL;
        }
                
        // Open file and check for errors
        int file; 
        if ((file = open(filepath, 'r')) == -1) {
            
            // Failed to open file
            fprintf(stderr, "Failed to map file: failed to open file\n");
            return NULL;
        }
        
        // Allocate memory for mapped file
        mapped_file = (mapped_file_t*)malloc(sizeof(mapped_file_t));
        if (!mapped_file) {
            
            // Failed to allocate memory
            fprintf(stderr, "Failed to map file: failed to allocate memory for mapped file\n");
            close(file);
            return NULL;
        }
        
        // Initialize mapped file fields
        mapped_file->file = file; 
        mapped_file->st_ino = file_stat.st_ino; 
        mapped_file->st_dev = file_stat.st_dev; 
        mapped_file->size = file_stat.st_size;
        
        // Map file contents to memory
        mapped_file->data = mmap(NULL, mapped_file->size, PROT_READ, MAP_SHARED, mapped_file->file, 0);
        if (mapped_file->data == MAP_FAILED) {
            
            // Failed to memory map file data
            fprintf(stderr, "Failed to map file: failed to map file data to memory\n");
            close(file); 
            free(mapped_file); 
            return NULL;
        }    
    }
    
    // Add the file to the end of the queue
    file_manager->mapped_file_queue[file_manager->rear] = mapped_file; 
    file_manager->rear = (file_manager->rear + 1) % file_manager->capacity; 
    file_manager->size++;
    
    // Release lock
    if (pthread_mutex_unlock(file_manager->lock) != SUCCESS) {
        
        // Failed to acquire lock
        fprintf(stderr, "Failed to map file: failed to release lock\n"); 
        return NULL;
    }
    
    // Successfully mapped new file
    return mapped_file;
}