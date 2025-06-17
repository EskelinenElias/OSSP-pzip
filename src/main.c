#include "../include/main.h"
#include <unistd.h>

// Cleanup routine
void cleanup_routine(cleanup_args_t* args) {
    if (args->workers && args->num_workers > 0) terminate_workers(args->workers, args->num_workers, args->queue);
    if (args->queue) free_task_queue(args->queue);
    if (args->tasks && args->num_tasks > 0) {
        for (size_t i = 0; i < args->num_tasks; i++) free_encoding_task(args->tasks + i);
    }
    if (args->boundary_task) free_encoding_task(args->boundary_task); 
    unmap_file(&args->file); 
    args->queue = NULL;
    args->workers = NULL; 
    args->num_workers = 0;
    args->tasks = NULL;
    args->num_tasks = 0;
    args->boundary_task = NULL;
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
    cleanup_args_t cleanup_args = {NULL, NULL, 0, NULL, 0, NULL};
        
    // Determine the number of available recources
    size_t num_workers = get_num_cores(); 
    
    // Initialize a task queue and check for errors
    task_queue_t* queue = create_task_queue(num_workers * 4); // Capacity is twice the number of workers
    if (!queue) {
        
        // Error; print error message and exit
        printf("pzip: failed to initialize task queue\n");
        return FAILURE;
    }
    
    // Add task queue to cleanup list
    cleanup_args.queue = queue; 

    // Initialize workers and check for errors
    pthread_t workers[num_workers];
    if (init_workers(workers, num_workers, queue) != SUCCESS) {
        
        // Error; print error message, free task queue, and exit
        printf("pzip: failed to initialize workers\n");
        cleanup_routine(&cleanup_args); // Free the task queue before exiting
        return FAILURE; // Error
    }
    
    // Add worker threads to cleanup list
    cleanup_args.workers = workers; 
    cleanup_args.num_workers = num_workers; 
    
    /* 3. PROCESS FILES ONE BY ONE TO CONSERVE MEMORY */

    // Initialize a pointer to encoded data for handling file boundaries
    encoding_task_t* file_boundary_task = NULL; 
    cleanup_args.boundary_task = file_boundary_task;

    // Loop trough the input files
    for (int i = 1; i < argc; i++) {

        /* 3.1. MAP FILE INTO MEMORY */

        // Map the file into memory and check for errors
        if (!argv[i]) continue; // Check that the argument is not NULL
        mapped_file_t file = map_file(argv[i]);
        if (file.data == NULL) {
            
            // Error; print error message, perform cleanup, and exit
            printf("pzip: cannot open file %s\n", argv[i]);
            cleanup_routine(&cleanup_args); 
            return FAILURE;
        }
        
        // Add mapped file to cleanup routine
        cleanup_args.file = file; 

        /* 3.2. ENCODE FILE CONTENTS */

        // Determine chunk sizea and the number of chunks and initialize an array for tasks
        size_t task_size = fmin(ceil((double)file.size / num_workers), MAX_CHUNK_SIZE);
        size_t num_tasks = ceil((double)file.size / task_size);
        encoding_task_t* tasks[num_tasks]; // Initialize an array for tasks
        
        // Add tasks list to cleanup
        cleanup_args.tasks = tasks[0];
        cleanup_args.num_tasks = num_tasks; 
        
        // Add tasks to the task queue
        size_t i = 0; 
        for (size_t t = 0; t < num_tasks; t++) {
            
            // Create a task
            tasks[t] = create_encoding_task(&file.data[i], fmin(task_size, file.size - i)); 
        
            // Add the task to the task queue to be processed (this will wait until there is room in the queue)
            add_task(queue, tasks[t]);
            
            // Increment the file index for the next chunk
            i += task_size;
        }
        
        /* 3.3. WRITE ENCODED DATA TO OUTPUT STREAM */
        
        // Wait for the first task to complete
        while (atomic_load(&tasks[0]->status) == IN_PROGRESS) usleep(1000);
        
        // If necessary, handle the boundary between the previous file and the current file
        if (file_boundary_task != NULL) {
            
            // Handle the boundary between the previous file and the current file
            handle_boundary(file_boundary_task->output, tasks[0]->output); 
            
            // Write the encoded data to the output stream and check for errors
            if (write_encoded_data_to_output(file_boundary_task->output) != SUCCESS) {
                
                // Error; perform cleanup routine and exit
                printf("Error writing encoded data to output stream\n");
                cleanup_routine(&cleanup_args); 
                return 1;
            }
            
            // Free the current task        
            free_encoding_task(file_boundary_task); 
            file_boundary_task = NULL; 
        }

        // Iterate over the processed chunks to handle boundaries and write to output stream
        for (int i = 1; i < num_tasks; i++) {
                        
            // Wait for the next task to complete
            while (tasks[i]->status == IN_PROGRESS) usleep(1000); 
                    
            // Handle the boundary between the current task and the next task
            handle_boundary(tasks[i - 1]->output, tasks[i]->output); 
                    
            // Write the encoded data to the output stream and check for errors
            if (write_encoded_data_to_output(tasks[i - 1]->output) != SUCCESS) {
                
                // Error; perform cleanup routine and exit
                printf("Error writing encoded data to output stream\n");
                cleanup_routine(&cleanup_args); 
                return 1;
            }       
      
            // Free the current task        
            free_encoding_task(tasks[i - 1]);
            tasks[i - 1] = NULL;
        }

        // Store the last task of this file to handle the boundary with the next file
        file_boundary_task = tasks[num_tasks - 1];
        tasks[num_tasks - 1] = NULL;

        /* 3.4. CLOSE THE CURRENT FILE */
        
        // Unmap the file after all tasks have processed
        unmap_file(&file);

    }
    
    /* 4. WRITE THE LAST PIECE OF ENCODED DATA TO OUTPUT STREAM */

    // Write the last piece of encoded data to output stream
    write_encoded_data_to_output(file_boundary_task->output); 
    
    // Free the boundary task
    free_encoding_task(file_boundary_task);
    file_boundary_task = NULL; 

    /* 5. TERMINATE THREADS AND CLEAN UP */
    
    // Send termination signal to all workers
    terminate_workers(workers, num_workers, queue);  

    // Free the task queue
    free_task_queue(queue);
    
    // Final cleanup 
    //cleanup_routine(&cleanup_args);

    return 0;
}