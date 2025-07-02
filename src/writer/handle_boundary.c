#include "../../include/writer/handle_boundary.h"

// Function to handle the boundary on subsequent results 
int handle_boundary(result_t* left, result_t* right) {
    
    // Check if the left and right object contain data
    if (left == NULL || right == NULL || left->capacity == 0 || right->capacity == 0) {
        
        // Invalid input
        fprintf(stderr, "Failed to handle boundary. One or both objects are NULL\n");
        return ERROR; 
    }

    // Get the last character from the left object
    char left_char = left->characters[left->capacity - 1]; 
    
    // Get the first character from the right object
    char right_char = right->characters[0]; 
    
    // Check if the left and right characters match
    if (left_char == right_char) {
                
        // Get the last count from the left object
        size_t* left_count = &left->counts[left->capacity - 1]; 
        
        // Get a pointer to the first count at the right object
        size_t* right_count = &right->counts[0]; 
        
        // Increase the count at the right object
        *right_count += *left_count; 
        *left_count = 0; 
        
        // Decrease the size of the left object
        left->capacity--; 
    }
    
    return SUCCESS; 
}

// EOF