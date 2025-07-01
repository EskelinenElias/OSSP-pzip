#include "../../include/task_data/free_task_data.h"

// Function to free task data
int free_task_data(task_data_t* task) {
        
    // Free allocated memory for task
    free(task);
    
    // Successfully freed task
    return SUCCESS;
}

// EOF