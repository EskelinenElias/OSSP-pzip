#include "../../include/result_data/free_result_data.h"

// Function to free the memory allocated for a result data structure
int free_result_data(result_data_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Failed to free result data: invalid input\n"); 
        return ERROR; 
    }
        
    // Free characters array
    if (result->characters) free(result->characters);
        
    // Free counts array
    if (result->counts) free(result->counts);

    // Free the data structure
    free(result); 
    
    // Successfully freed result data
    return SUCCESS; 
}

// EOF