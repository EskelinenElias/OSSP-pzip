#include "../../include/writer/free_writer_thread_resources.h"

// Function to free writer thread resources
int free_writer_thread_resources(writer_thread_resources_t* resources) {
    
    // Input validation
    if (!resources) {
        
        // Invalid input
        fprintf(stderr, "Failed to free writer resources: invalid input\n");
        return ERROR;
    }
    
    // Free memory allocated for writer resources
    if (resources->current_result) free_result(resources->current_result); 
    if (resources->next_result) free_result(resources->next_result);
    
    // Free memory allocated for writer resources structure
    free(resources);
    
    // Successfully freed memory for writer resources
    return SUCCESS;
}

// EOF 