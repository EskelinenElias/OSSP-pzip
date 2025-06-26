#ifndef MAPPED_FILE_H   
#define MAPPED_FILE_H   

#include <stdio.h>

// Mapped file structure; represents the data of a mapped file
typedef struct {
    size_t st_dev;
    size_t st_ino;
    char *data;
    size_t size;
    int file;
} mapped_file_t;

#endif // MAPPED_FILE_H