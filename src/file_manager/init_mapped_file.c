#include "../../include/file_manager/init_mapped_file.h"

// Function to initialize a mapped file (a memory mapped file)
mapped_file_t* init_mapped_file(int file_descriptor, struct stat* file_stat) {
    
    // Input validation
    if (file_descriptor < 0 || file_stat->st_size <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize mapped file: invalid input\n");
        return NULL;
    }
    
    // Allocate memory for the mapped file structure
    mapped_file_t* mapped_file = malloc(sizeof(mapped_file_t));
    if (!mapped_file) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to initialize mapped file: failed to allocate memory for mapped file\n");
        return NULL;
    }
    
    // Map the file into memory
    mapped_file->data = mmap(NULL, file_stat->st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0);
    if (mapped_file->data == MAP_FAILED) {
        
        // Failed to initialize mapped file to memory
        fprintf(stderr, "Failed to initialize mapped file: failed to map file to memory\n");
        free(mapped_file);
        return NULL;
    }
    
    // Initialize the mapped file structure
    mapped_file->st_ino = file_stat->st_ino;
    mapped_file->st_dev = file_stat->st_dev;
    mapped_file->size = file_stat->st_size;
    
    // Successfully mapped file
    return mapped_file;
}

// EOF