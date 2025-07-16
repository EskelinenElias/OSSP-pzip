#include "../../include/encoded_data/free_encoded_data.h"

// Function to free encoded data structure
int free_encoded_data(encoded_data_t* encoded_data) {
    
    // Input validation
    if (!encoded_data) {
        
        // Invalid input
        fprintf(stderr, "Failed to freeencoded datadata: invalid input\n"); 
        return ERROR; 
    }
        
    // Free characters array
    if (encoded_data->characters) free(encoded_data->characters);
        
    // Free counts array
    if (encoded_data->counts) free(encoded_data->counts);

    // Free encoded data structure
    free(encoded_data); 
    
    // Successfully freed encoded data
    return SUCCESS; 
}

// EOF