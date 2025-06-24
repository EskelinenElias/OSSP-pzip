#include "../include/file_manager.h"

// Function to initialize file manager
file_manager_t* init_file_manager(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize file manager. Invalid input\n");
        return NULL;
    }
    
    // Allocate memory for file manager
    file_manager_t* manager = (file_manager_t*)malloc(sizeof(file_manager_t));
    if (!manager) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to allocate memory for file manager\n");
        return NULL;
    }
    
    // Allocate memory for ordered files array
    manager->ordered_files = (mapped_file_t**)malloc(capacity * sizeof(mapped_file_t*));
    if (!manager->ordered_files) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to allocate memory for ordered files array\n");
        free(manager);
        return NULL;
    }
    
    // Allocate memory for mapped files array
    manager->mapped_files = (mapped_file_t*)malloc(capacity * sizeof(mapped_file_t));
    if (!manager->mapped_files) {
        
        // Failed to allocate memory
        fprintf(stderr, "Failed to allocate memory for mapped files array\n");
        free(manager->ordered_files);
        free(manager);
        return NULL;
    }
    
    // Set initial values
    manager->num_mapped_files = 0;
    manager->num_ordered_files = 0;
    manager->capacity = capacity;
    
    // Successfully initialized file manager
    return manager;
}

// Function to free file manager
file_manager_t* free_file_manager(file_manager_t* manager) {
    
    // Input validation
    if (!manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to free file manager. Invalid input\n");
        return NULL;
    }
    
    // Unmap all files 
    for (int i = 0; i < manager->num_mapped_files; i++) {
        
        // Get pointer to a mapped file
        mapped_file_t* mapped_file = &manager->mapped_files[i];
        
        // Unmap file data
        if (mapped_file->data) munmap(mapped_file->data, mapped_file->size);
        
        // Close file descriptor
        if (mapped_file->file != -1) close(mapped_file->file);
        
        // Reset fields
        mapped_file->data = NULL; 
        mapped_file->file = -1;
        mapped_file->size = 0;
        mapped_file->st_dev = 0;
        mapped_file->st_ino = 0;
    }
    
    // Free arrays
    if (manager->mapped_files)free(manager->mapped_files);
    if (manager->ordered_files)free(manager->ordered_files);
    
    // Free manager
    free(manager);
    
    // Successfully freed file manager
    return NULL; 
}

// Function to check if a file is mapped
mapped_file_t* is_mapped(file_manager_t* file_manager, const char* filepath) {
    
    // Input validation
    if (!filepath || !file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to check if file is mapped. Invalid input\n");
        return NULL;
    }
    
    // Get file stats and check for errors
    struct stat file_stat;
    if (access(filepath, F_OK) == -1 && stat(filepath, &file_stat) == -1) {
        
        // File does not exist
        fprintf(stderr, "Failed to get file stats");
        return NULL;
    }
    
    // Check if the file is already mapped
    for (int i = 0; i < file_manager->num_mapped_files; i++) {
        
        // Get pointer to a mapped file
        mapped_file_t* mapped_file = &file_manager->mapped_files[i];
        
        // Check if file stats match the already mapped file
        if (mapped_file->st_dev == file_stat.st_dev && mapped_file->st_ino == file_stat.st_ino) {
            
            // File is already mapped
            return mapped_file;
        }
    }
    
    // File is not mapped
    return NULL;
}

// Function to map a file into memory
mapped_file_t* map_next_file(file_manager_t* file_manager, const char *filepath) {
    
    // Input validation
    if (!filepath || !file_manager) {
        
        // Invalid input
        fprintf(stderr, "Failed to map file. Invalid input\n");
        return NULL;
    }
    
    // Check that the file manager has room 
    if (file_manager->num_mapped_files == file_manager->capacity) {
        
        // File manager is full
        fprintf(stderr, "Failed to map file. File manager is full");
        return NULL;
    }
    
    // Check if the file exists
    if (access(filepath, F_OK) == -1) {
        
        // File does not exist
        fprintf(stderr, "Failed to map file. File does not exist");
        return NULL;
    }
            
    // Get file stats and check for errors
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        
        // Failed to get file size
        perror("Failed to get file size"); 
        return NULL;
    }
    
    // Check if the file has already been mapped
    for (size_t i = 0; i < file_manager->num_mapped_files; i++) {
        
        // Get existing mapped file
        mapped_file_t* already_mapped_file = &file_manager->mapped_files[i];
        
        // Compare file stats
        if (already_mapped_file->st_ino == file_stat.st_ino && already_mapped_file->st_dev == file_stat.st_dev) {
                    
            // File has already been mapped; add pointer to the existing mapping to the ordered list
            file_manager->ordered_files[file_manager->num_ordered_files] = already_mapped_file; 
            file_manager->num_ordered_files++; 
            
            // Return a pointer to the already mapped file
            return already_mapped_file;
        }
    }
    
    // Open file and check for errors
    int file; 
    if ((file = open(filepath, 'r')) == -1) {
        
        // Failed to open file
        fprintf(stderr, "Failed to map file. Failed to open file\n");
        return NULL;
    }
    
    // File has not been mapped; get pointer to the next mapped file slot
    mapped_file_t* next_mapped_file = &file_manager->mapped_files[file_manager->num_mapped_files];
    file_manager->num_mapped_files++;
    
    // Initialize new mapped file fields
    next_mapped_file->file = file; 
    next_mapped_file->size = file_stat.st_size; 
    next_mapped_file->st_ino = file_stat.st_ino;
    next_mapped_file->st_dev = file_stat.st_dev;
    
    // Memory map the file data
    next_mapped_file->data = mmap(NULL, next_mapped_file->size, PROT_READ, MAP_PRIVATE, next_mapped_file->file, 0);
    if (next_mapped_file->data == MAP_FAILED) {
        
        // Failed to memory map file data
        fprintf(stderr, "Failed to memory map file data\n");
        return NULL;
    }    
    
    // Add the new mapped file to the ordered list
    file_manager->ordered_files[file_manager->num_ordered_files] = next_mapped_file;
    file_manager->num_ordered_files++;
    
    // Successfully mapped new file
    return next_mapped_file;
}

// Function to unmap a file from memory
mapped_file_t* unmap_next_file(file_manager_t* file_manager) {
    
    // Input validation
    if (!file_manager || !file_manager->mapped_files || !file_manager->ordered_files) {
        
        // Invalid input
        fprintf(stderr, "Failed to unmap next file. Invalid input\n");
        return NULL;
    }
    
    // Get the next mapped file in order
    mapped_file_t* next_mapped_file = NULL; 
    for (size_t i = 0; i < file_manager->num_ordered_files; i++) {
        
        // Get the first mapped file
        if (!next_mapped_file && file_manager->ordered_files[i]) {
            
            // Set the next mapped file
            next_mapped_file = file_manager->ordered_files[i];
            
            // Set the element in the array to NULL
            file_manager->ordered_files[i] = NULL;
        }
        
        // If a pointer to the same mapped file is found, set the mapped file to NULL
        if (next_mapped_file && next_mapped_file == file_manager->ordered_files[i]) {
            
            // Set the next mapped file to null and break
            next_mapped_file = NULL;
            break;
        }
    }
    
    // Check if the file was found and should be unmapped
    if (next_mapped_file) {
        
        // If the mapped file has data, unmap it from memory
        if (next_mapped_file->data) {
            
            // Unmap the file from memory
            munmap(next_mapped_file->data, next_mapped_file->size);
        }
        
        // If the mapped file is open, close it
        if (next_mapped_file->file != -1) {
            
            // Close the file descriptor
            close(next_mapped_file->file);
        }
        
        // Reset mapped file fields to NULL
        next_mapped_file->data = NULL;
        next_mapped_file->size = 0;
        next_mapped_file->file = -1;
        next_mapped_file->st_dev = 0;
        next_mapped_file->st_ino = 0;
    }
    
    // Successfully unmapped file
    return NULL;
}
