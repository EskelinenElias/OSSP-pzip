#include "../../include/file_manager/init_file_manager.h"

file_manager_t* init_file_manager(size_t capacity) {
    
    // Input validation
    if (capacity <= 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to initialize file manager: invalid capacity\n");
        return NULL;
    }
    
    // Allocate memory for file manager
    file_manager_t* file_manager = (file_manager_t*)malloc(sizeof(file_manager_t));
    if (!file_manager) {
        
        // Failed to allocate memory for file manager
        fprintf(stderr, "Failed to initialize file manager: failed to allocate memory for file manager\n");
        return NULL;
    }
    
    // Initialize file manager fields
    file_manager->capacity = capacity;
    file_manager->size = 0;
    file_manager->front = 0; 
    file_manager->rear = 0; 

    // Allocate memory for mapped file queue
    if (!(file_manager->mapped_file_queue = (mapped_file_t**)malloc(capacity * sizeof(mapped_file_t*)))) {
        
        // Failed to allocate memory for mapped file queue
        fprintf(stderr, "Failed to initialize file manager: failed to allocate memory for mapped file queue\n");
        free(file_manager);
        return NULL;
    }
    
    // Allocate memory for and initialize file manager mutex lock
    if (!(file_manager->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)))) {
        
        // Failed to allocate memory for file manager mutex lock
        fprintf(stderr, "Failed to initialize file manager: failed to allocate memory for file manager mutex lock\n");
        free(file_manager->mapped_file_queue);
        free(file_manager);
        return NULL;
        
    } else if (pthread_mutex_init(file_manager->lock, NULL) != SUCCESS) {
        
        // Failed to initialize file manager mutex lock
        fprintf(stderr, "Failed to initialize file manager: failed to initialize file manager mutex lock\n");
        free(file_manager->mapped_file_queue);
        free(file_manager->lock);
        free(file_manager);
        return NULL;
    }

    // Successfully initialized file manager
    return file_manager;
}
