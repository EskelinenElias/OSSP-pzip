#include "../../include/task/free_task.h"

// Function to free task
int free_task(task_t* task) {
        
    // Free allocated memory for task
    free(task);
    
    // Successfully freed task
    return SUCCESS;
}

// EOF