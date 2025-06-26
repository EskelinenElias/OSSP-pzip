#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

// Mapped file structure; represents the data of a mapped file
typedef struct {
    size_t st_dev;
    size_t st_ino;
    char *data;
    size_t size;
    int file;
} mapped_file_t;

// File manager structure; manages mapped files
typedef struct file_manager_t {
    pthread_mutex_t* lock; 
    size_t num_ordered_files;
    size_t num_mapped_files;
    size_t capacity;  
    mapped_file_t** ordered_files; 
    mapped_file_t* mapped_files; 
} file_manager_t;

// Function to initialize file manager
file_manager_t* init_file_manager(size_t capacity); 

// Function to free file manager
file_manager_t* free_file_manager(file_manager_t* manager); 

// Function to map a file into memory
mapped_file_t* map_next_file(file_manager_t* file_manager, const char *filepath);

// Function to unmap a file from memory
mapped_file_t* unmap_next_file(file_manager_t* file_manager); 