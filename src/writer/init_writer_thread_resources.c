#include "../../include/writer/init_writer_thread_resources.h"

// Function to initialize writer thread resources
writer_thread_resources_t* init_writer_thread_resources() {
    
    // Allocate memory for writer thread resources
    writer_thread_resources_t* resources = calloc(1, sizeof(writer_thread_resources_t));
    if (!resources) {
        
        // Failed to allocate memory for writer thread resources
        fprintf(stderr, "Failed to initialize writer thread resources: failed to allocate memory for writer thread resources\n");
        return NULL;
    }
    
    // Successfully allocated memory for writer thread resources
    return resources;
}

// EOF