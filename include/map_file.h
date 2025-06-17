#ifndef MAP_FILES_H
#define MAP_FILES_H

#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Mapped file structure; represents a file mapped into memory
typedef struct {
    char *data;
    size_t size;
    int file;
} mapped_file_t;

// Map a single file into memory
mapped_file_t map_file(const char *filepath);

// Unmap a single file
void unmap_file(mapped_file_t *mapping);

#endif // MAP_FILES_H