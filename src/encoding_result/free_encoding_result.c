#include "../../include/encoding_result/free_encoding_result.h"

// Function to free encoded data structure
int free_encoding_result(encoding_result_t* encoding_result) {
    
    // Input validation
    if (!encoding_result) {
        
        // Invalid input
        fprintf(stderr, "Failed to freeencoded datadata: invalid input\n"); 
        return ERROR; 
    }
        
    // Free characters array
    if (encoding_result->characters) free(encoding_result->characters);
        
    // Free counts array
    if (encoding_result->counts) free(encoding_result->counts);

    // Free encoded data structure
    free(encoding_result); 
    
    // Successfully freed encoded data
    return SUCCESS; 
}

// EOF