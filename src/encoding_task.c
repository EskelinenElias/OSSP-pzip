#include "../include/encoding_task.h"
#include <stdatomic.h>

// Function to create a new RLE compression task
encoding_task_t* create_encoding_task(char* input, size_t length) {
    
    // Allocate memory for the encoding task structure
    encoding_task_t* task = malloc(sizeof(encoding_task_t)); 
    if (task == NULL) return NULL; 
    
    // Initialize the task fields
    atomic_init(&task->status, IN_PROGRESS);
    task->input = input; 
    task->length = length; 
    task->output = NULL; 
    
    // Successfully created the task
    return task; 
}

// encoding_task_t create_encoding_task_and_allocate_output(char* input, size_t length, size_t init_output_size) {
    
//     // Allocate memory for the encoding task structure
//     encoding_task_t* task = malloc(sizeof(encoding_task_t)); 
//     if (task == NULL) return NULL; 
    
//     // Allocate memory for the output encoded data
//     task->output = create_encoded_data(init_output_size);
//     if (task->output == NULL) { free(task); return NULL; } 
//     return task; 
// }

// Function to create an empty encoding task 
encoding_task_t* create_empty_encoding_task() {
    encoding_task_t* task = malloc(sizeof(encoding_task_t));
    task->input = NULL; 
    task->length = 0; 
    task->output = NULL; 
    return task;
}

// Function to free the memory allocated for an encoding task structure
void free_encoding_task(encoding_task_t* task) {
    if (task == NULL) return; 
    free_encoded_data(task->output);
    free(task);
}

// Function to generate a termination task 
encoding_task_t* create_termination_task() {
    return NULL; 
}