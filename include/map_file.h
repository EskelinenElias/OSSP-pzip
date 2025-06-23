#ifndef MAP_FILES_H
#define MAP_FILES_H

#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

// Mapped file structure; represents a file mapped into memory
typedef struct {
    char *data;
    size_t size;
    int file;
} mapped_file_t;

typedef struct {
    mapped_file_t** files;
    size_t num_files;
} mapped_files_t;

// Map a single file into memory
mapped_file_t* map_file(const char *filepath);

// Unmap a single file
mapped_file_t* unmap_file(mapped_file_t *mapping);

#endif // MAP_FILES_H