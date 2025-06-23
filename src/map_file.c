#include "../include/map_file.h"

// Function to map a file into memory
mapped_file_t* map_file(const char *filepath) {
    
    // Input validation
    if (!filepath) {
        
        // Invalid filepath
        fprintf(stderr, "Invalid filepath\n");
        return NULL;
    }
    
    // Allocate memory for the mapped file structure and check for errors
    mapped_file_t* mapped_file = malloc(sizeof(mapped_file_t));
    if (!mapped_file) {
        
        // Failed to allocate memory for mapped file structure
        fprintf(stderr, "Failed to allocate memory for mapped file structure\n");
        return NULL;
    }
    
    // Initialize mapped file structure
    mapped_file->file = -1;
    mapped_file->data = NULL;
    mapped_file->size = 0;
    
    // Check if the file exists
    if (access(filepath, F_OK) == -1) {
        
        // File does not exist
        perror("File does not exist");
        unmap_file(mapped_file);
        return NULL;
    }
    
    // Open file and check for errors
    mapped_file->file = open(filepath, 'r');
    if (mapped_file->file == -1) {
        
        // Failed to open file
        perror("Failed to open file\n");
        unmap_file(mapped_file);
        return NULL;
    }
    
    // Get file size
    struct stat file_stat;
    if (fstat(mapped_file->file, &file_stat) == -1) {
        
        // Failed to get file size
        perror("Failed to get file size");
        unmap_file(mapped_file);
        return NULL;
    }
    mapped_file->size = file_stat.st_size;
    
    // Map the file
    mapped_file->data = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, mapped_file->file, 0);
    if (mapped_file->data == MAP_FAILED) {
        
        // Failed to map file
        perror("Failed to map file");
        unmap_file(mapped_file);
        return NULL;
    }
    
    // Successfully mapped file
    return mapped_file;
}

// Function to unmap a file from memory
mapped_file_t* unmap_file(mapped_file_t* mapping) {
    
    // Check for null pointer
    if (!mapping) return NULL;
    
    // Unmap and close file 
    if (mapping->data) {
        
        // Unmap file
        if (munmap(mapping->data, mapping->size) != 0) {
            
            // Failed to unmap file
            fprintf(stderr, "Failed to unmap file\n"); 
        };
    }
    
    // Close file
    if (mapping->file != -1) close(mapping->file); 
    
    // Reset pointers
    mapping->data = NULL;
    mapping->size = 0;
    mapping->file = -1;
    free(mapping);
    return NULL; 
}