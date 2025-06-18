#include "../include/main.h"

// Cleanup routine
void cleanup_routine(cleanup_args_t* args) {
    if (args->workers && args->num_workers > 0) terminate_workers(args->workers, args->num_workers, args->manager);
    if (args->manager) free_manager(args->manager);
    if (args->current_result) free_result(args->current_result);
    if (args->next_result) free_result(args->next_result); 
    unmap_file(&args->file); 
    args->manager = NULL;
    args->workers = NULL; 
    args->num_workers = 0;
    args->current_result = NULL;
    args->next_result = NULL;
}

// Main function 
int main(int argc, char *argv[]) {

    /* 1. VALIDATE INPUT */

    // Check that the correct number of arguments were provided
    if (argc < 2) {
        fprintf(stdout, "pzip: file1 [file2 ...]\n");
        return 1;
    }

    /* 2. INITIALIZE CLEANUP ROUTINE, TASK QUEUE AND THREADS */ 
    
    // Initialize cleanup arguments
    cleanup_args_t process = {NULL, NULL, 0, NULL, 0, NULL};
        
    // Determine the number of available recources
    
    // Initialize a task queue and check for errors
    process.manager = init_manager(MAX_CAPACITY);
    if (!process.manager) {
        
        // Error; print error message and exit
        fprintf(stderr, "Failed to initialize task manager\n");
        return ERROR;
    }

    // Initialize workers and check for errors
    process.num_workers = get_num_cores(); 
    process.workers = init_workers(process.num_workers, process.manager); 
    if (!process.workers) {
        
        // Failed to initialize workers
        fprintf(stderr, "Failed to initialize workers\n");
        cleanup_routine(&process); 
        return ERROR;
    }
    
    /* 3. PROCESS FILES ONE BY ONE TO CONSERVE MEMORY */

    // Loop trough the input files
    for (int i = 1; i < argc; i++) {

        /* 3.1. MAP FILE INTO MEMORY */

        // Map the file into memory and check for errors
        if (!argv[i]) continue; // Check that the argument is not NULL
        process.file = map_file(argv[i]);
        if (process.file.data == NULL) {
            
            // Error; print error message, perform cleanup, and exit
            printf("Failed to open file %s\n", argv[i]);
            cleanup_routine(&process); 
            return ERROR;
        }

        /* 3.2. ENCODE FILE CONTENTS */

        // Determine chunk size and the number of chunks and initialize an array for tasks
        size_t task_size = fmin(ceil((double)process.file.size / process.num_workers), MAX_CHUNK_SIZE);
        size_t num_tasks = ceil((double)process.file.size / task_size);
        
        // Add tasks to the task queue
        size_t i = 0; 
        for (size_t t = 0; t < num_tasks; t++) {
            
            // Add the task to the task manager
            yield_task(process.manager, process.file.data + i, fmin(task_size, process.file.size - i));

            // Increment the file index for the next chunk
            i += task_size;
        }
        
        /* 3.3. WRITE ENCODED DATA TO OUTPUT STREAM */

        // Iterate over the processed chunks to handle boundaries and write to output stream
        while ((process.next_result = claim_result(process.manager)) != NULL) {
            
            if (process.current_result) {
                
                // Handle the boundary between the current task and the next task
                handle_boundary(process.current_result, process.next_result); 
                        
                // Write the encoded data to the output stream and check for errors
                if (write_encoded_data_to_output(process.current_result) != SUCCESS) {
                    
                    // Error; perform cleanup routine 
                    printf("Error writing encoded data to output stream\n");
                    cleanup_routine(&process); 
                    return ERROR;
                }       
      
                // Free the current result        
                free_result(process.current_result);
            }
            
            // Swap the next result to the current result
            process.current_result = process.next_result;
        }

        /* 3.4. CLOSE THE CURRENT FILE */
        
        // Unmap the file after all tasks have processed
        unmap_file(&process.file);

    }
    
    /* 4. WRITE THE LAST PIECE OF ENCODED DATA TO OUTPUT STREAM */

    // Write the last piece of encoded data to output stream
    write_encoded_data_to_output(process.next_result); 
    
    // Free the boundary task
    free_result(process.next_result);

    /* 5. TERMINATE THREADS AND CLEAN UP */
    
    // Send termination signal to all workers
    cleanup_routine(&process);

    return 0;
}