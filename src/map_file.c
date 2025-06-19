#include "../include/map_file.h"

// Function to map a file into memory
mapped_file_t* map_file(const char *filepath) {
    mapped_file_t* result = malloc(sizeof(mapped_file_t));
    
    // Open file
    result->file = open(filepath, 'r');
    if (result->file == -1) {
        return result;
    }
    
    // Get file size
    struct stat file_stat;
    if (fstat(result->file, &file_stat) == -1) {
        close(result->file);
        return result;
    }
    
    // Map the file
    result->data = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, result->file, 0);
    if (result->data == MAP_FAILED) {
        close(result->file);
        result->data = NULL;
        return result;
    }
    
    result->size = file_stat.st_size;
    return result;
}

// Function to unmap a file from memory
void unmap_file(mapped_file_t* mapping) {
    
    // Check for null pointer
    if (!mapping) return;
    
    // Unmap data
    if (mapping->data) {
        
        // Unmap data and close file
        munmap(mapping->data, mapping->size);
        if (mapping->file != -1) {
            close(mapping->file);
        }
    }
    
    // Reset pointers
    mapping->data = NULL;
    mapping->size = 0;
    mapping->file = -1;
    
    free(mapping);
}