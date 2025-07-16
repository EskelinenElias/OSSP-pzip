#include "../../include/encoding_task/free_encoding_task.h"

// Function to free encoding task
int free_encoding_task(encoding_task_t* encoding_task) {
        
    // Free memory allocated for encoding task
    free(encoding_task);
    
    // Successfully freed encoding task
    return SUCCESS;
}

// EOF