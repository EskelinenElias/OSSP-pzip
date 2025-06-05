#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/constants.h"

#define INIT_CAPACITY 16

typedef struct {
    int is_ready; 
    char* characters;
    size_t* counts;
    size_t size;
    size_t capacity;
} encoded_data_t;

encoded_data_t* create_encoded_data(size_t initial_capacity);
int realloc_encoded_data(encoded_data_t* encoded_data, size_t new_capacity);
void free_encoded_data(encoded_data_t* encoded_data);
int append_to_encoded_data(encoded_data_t* encoded_data, char character, size_t count);

#endif // DATA_H