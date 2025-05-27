#include "../include/mmap_file.h"

mapped_file_t map_file(const char *filepath) {
    mapped_file_t result = {NULL, 0, -1};
    
    // Open file
    result.file = open(filepath, O_RDONLY);
    if (result.file == -1) {
        return result;
    }
    
    // Get file size
    struct stat file_stat;
    if (fstat(result.file, &file_stat) == -1) {
        close(result.file);
        return result;
    }
    
    // Map the file
    result.data = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, result.file, 0);
    if (result.data == MAP_FAILED) {
        close(result.file);
        result.data = NULL;
        return result;
    }
    
    result.size = file_stat.st_size;
    return result;
}

void unmap_file(mapped_file_t *mapping) {
    if (!mapping || !mapping->data) return;
    
    munmap(mapping->data, mapping->size);
    if (mapping->file != -1) {
        close(mapping->file);
    }
    
    mapping->data = NULL;
    mapping->size = 0;
    mapping->file = -1;
}