#include "../../include/writer_thread/write_to_output.h"

// Function to write encoding result to standard output as bytes
int write_encoding_result_to_output(encoding_result_t* result) {
    
    // Input validation
    if (!result) {
        
        // Invalid input
        fprintf(stderr, "Failed to write encoding result to output: invalid input\n");
        return ERROR; 
    }

    // Iterate through the encoded data
    for (int i = 0; i < result->capacity; i++) {
        
        // Write count and character to output
        fwrite(&result->counts[i], sizeof(int), 1, stdout); 
        fwrite(&result->characters[i], sizeof(char), 1, stdout);    
    }
    
    // Successfully wrote encoding result to output
    return SUCCESS; 
}

// // Function to write encoded data to standard output as text (for testing purposes)
// int write_encoded_text_to_output(encoding_result_t* result) {
    
//     // Input validation
//     if (!result) {
        
//         // Invalid input
//         fprintf(stderr, "Failed to write result to output. Invalid input\n");
//         return ERROR; 
//     }

//     // Process the data
//     for (int i = 0; i < result->capacity; i++) {
                
//         // Write count and character to buffer and advance buffer
//         fprintf(stdout, "%lu%c", result->counts[i], result->characters[i]); 
//     }

//     return SUCCESS; 
// }

// EOF 