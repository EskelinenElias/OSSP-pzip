# The Parallel Zip Tool 

The parallel zip (pzip) tool compresses one or more input files using run length encoding. It uses parallel processing to increase compression speed. 

## Usage

The tool can be used to run length encode one or more files to a single output file. If multiple files are encoded into one output file, file boundary information can't be recovered from the encoded file. The tool can be run with the command `./pzip file1 [file2 ...]`. The output can be directed to another file (here, `output_file`) by changing the command to `./pzip file1 [file2 ...] > output_file`. 

## Project structure

The project is structured as follows: 
- Source code can be found in the directory `src` and the corresponding header files in the directory `include`. 
- Compiled code goes in the `obj`-directory. 
- Project can be compiled by running the command `make` in the root directory. This compiles the project according to the instructions in `Makefile` in the root directory.
- Tests can be found from the `tests`-directory. Automated tests can be run from the root directory by running the command `make test` in the root directory. 

## Implementation 

The algorithm utilizes parallel processing to speed up the encoding process. The main thread spawns worker threads and a specialized writer thread. The number of worker threads is determined from the number of cores available; the minimum number of worker threads is 1. In total, the minimum number of threads is then 3: 1 main thread, 1 writer thread, and at least 1 worker thread.

An input file is mapped into memory and then the main thread divides the data in the file to 'encoding tasks' (see section Encoding task). An encoding task is a chunk of input data. The tasks are put into a task queue (see section Task queue), from where the worker threads claim the task and process them. 

Files are mapped into memory for memory efficient processing. Files are mapped dynamically to minimize worker thread downtime: the program moves to the next file as soon as it finishes generating tasks from the current file, and files are unmapped when they are no longer needed. A special file manager is responsible for managing the file operations (see section File manager).

Worker threads encode the data in each task using run-length encoding, and then pass the result to a result queue (see section Result queue). The result queue makes sure that the results remain in the correct order. The writer thread claims results from the result queue as they become available, and writes them to the output stream.

Race conditions are avoided by using a mutex locks. Dividing the input data into tasks means that work is distributed dynamically. Memory management is also done dynamically to minimize memory usage and to simplify memory ownership between threads. Busy waiting is avoided by using condition variables. 

The separate writer thread enables writing the results to output as they are processed. The idea behind the implementation is to get the write-to-output operation going as fast as possible; as the process is IO-bound, this should offer some performance gains. 

### Main thread

The main function parses and validates the input arguments, initializes all needed components (file manager, tasks queue, worker threads, results queue, and the writer thread) (see section "Main thread resources), uses the File manager to open and memory map input files, and splits the input files to "tasks" (see Encoding task); these are simply chunks of input data of a set size. The main thread then adds these tasks to the task queue (see Task queue) to be processed by the worker threads (see Worker thread pool). In the end, or in the case of an error, the main thread performs cleanup operations. 

### Main thread resources

The main thread resources are managed by the `main_thread_resources_t` structure. It stores pointers to all allocated data structures, and allows for easy initialization of resources and cleanup in the end or in the case of an error. The structure can be initialized by calling the `init_main_thread_resources` function, and freed using the `free_main_thread_resources` function. The structure stores pointers to the file manager, task queue, result queue, worker thread pool and writer thread. 

### File manager

The file manager manages the input files. It maps the input files to memory, keeps track of mapped files, and unmaps files when they are no longer needed. It also provides a mechanism to unmap the next file when an EOF result is encountered. The most important job of the file manager is to make sure that no attempt is made to map a file to memory more than once, as this would lead to undefined behavior. 

The file manager is implemented as a `file_manager_t` structure. It can be initialized by calling the `init_file_manager` function, and freed using the `free_file_manager` function. The structure stores an array of mapped files and a mutex to ensure thread safety. Mapped files are stored in the order they are added, in a queue; this way, they can also be unmapped in the same order.

Mapped files are stored as `mapped_file_t` structures. These can be initialized (mapped) by calling the `init_mapped_file` function, and freed using the `free_mapped_file` function. The structure stores the file descriptor, the file size, and a pointer to the mapped memory and file stats to uniquely identify the file (the inode number and device ID).

The function `map_next_file` takes a `file_manager_t` pointer and a filepath. It checks that the file exists, is accessible and is not already mapped; if the file is already mapped, the function returns a pointer to the already created `mapped_file_t` structure. The file is also moved to the end of the queue to delay unmapping, and the original index is set to `NULL` (this indicates to the function `unmap_next_file` that no file needs to be unmapped). If the file is not mapped, it is mapped and added to the end of the queue.

The function `unmap_next_file` takes a `file_manager_t` pointer and returns an integer (return code). The function checks if the next spot in the mapped file queue (pointer by the `head` index) contains a mapped file. If it does, the file is unmapped and removed from the queue. If the queue is empty, the function simply returns. 

### Encoding task 

The encoding task data structure represents an encoding task: a chunk of input data to be encoded. It contains a character array pointer and size of the array. Function `init_encoding_task` can be used to initialize the encoding task data structure, and function `free_encoding_task` can be used to free the encoding task data structure. Initiating a task also reserves a spot for the task's result in the results queue using the `reserve_spot` function. 

### Task queue

The task queue data structure represents a queue of encoding tasks. It contains a pointer to the head and tail of the queue, and the current size of the queue. Function `init_task_queue` can be used to initialize the task queue data structure, and function `free_task_queue` can be used to free the task queue data structure. Thread safety is ensured by utilizing a mutex lock. 

Tasks can be added to the queue by passing task data to function `yield_task`; this function initiates a new encoding task data structure and adds a pointer to it to the end of the queue. The main thread is responsible for populating the queue, and the worker threads can claim tasks from the queue by calling function `claim_task`. Task order is preserved by reserving a spot for each tasks result in the result queue; this way the writer can write the results to output in the correct order. Memory ownership of task data is transferred to the worker thread, as it claims a task. 

Function `yield_NULL_task` can be used to yield a `NULL` task, which acts as a termination signal for the worker threads. Thread safety is ensured by utilizing a mutex lock. 

### Encoding result 

The encoding result data structure represents a chunk of encoded data. It contains a character array, a character counts array and the current size of these arrays. Function `init_encoding_result` can be used to initialize the encoding result data structure, function `free_encoding_result` can be used to free the encoding result data structure, and function `resize_encoding_result` can be used to resize (reallocate) the encoding result data structure.

### Results queue

The results queue manages encoding results and makes sure that the results are processed in the correct order. It contains a pointer to the head and tail of the queue, and the current size of the queue. Function `init_result_queue` can be used to initialize the result queue data structure, and function `free_result_queue` can be used to free the result queue data structure. Thread safety is ensured by utilizing a mutex lock.

Worker threads yield results to the results queue by passing encoded data using function `yield_result`. In order to yield a result to the results queue, an index to the result queue must first be reserved, acting as a kind of ticket to access the queue. This index is reserved when initializing a task (see section Encoding task). As a result is yielded to the result queue, the worker thread gives up ownership of the memory, and it's pointer to the memory allocated for the result is set to `NULL`.

The writer thread can then claim these results by calling function `claim_result`. Results are claimed in order; this ensures that they are written to output in the correct order. As a result is claimed, the ownership of that memory is transferred to the writer thread, and the pointer to that result in the result queue is set to `NULL`.

Result status is tracked by storing status flags in a separate array. These flags are updated as the task is processed: when reserving a spot in the results queue, the status goes from `EMPTY` to `RESERVED`; when yielding a result, the status goes from `RESERVED` to `COMPLETED`; when claiming a result, the status goes from `COMPLETED` back to `EMPTY`, as the spot in the queue is now free. 

To terminate the writer thread, a `NULL` result can be yielded to the result queue. The writer thread will terminate upon claiming the `NULL` result. Yielding a `NULL` result is done by calling `yield_NULL_result`, and does not require reserving an index from the queue beforehand.

### Worker thread pool 

The worker thread pool manages worker threads. Worker thread pool can be initialized using `init_worker_thread_pool` and terminated using `free_worker_thread_pool`. The minimun number of workers is 1. The worker pool stores worker thread data as a `worker_thread_t` data structure, which holds the thread's ID, the thread's input arguments, and all the thread's resources; this enables the worker's resources to be freed by the main thread after termination.

A worker thread can be initialized by calling the function `init_worker_thread`. The worker thread's resources can be freed after termination by calling the function `free_worker_thread`. Worker threads run the function `worker_thread_main`, and take as argument a `worker_thread_t` data structure.

The function `worker_thread_main` runs a loop, which claims a task (by calling function `claim_task`) from the task queue, encodes the data and returns the result to the results queue (by calling function `yield_result`). The task contains a reserved index to the result queue, which is used to yield the result to the results queue. If the task is `NULL`, the worker thread terminates.

### Writer thread

The writer thread has multiple jobs: it claims encoding results from the results queue; it handles boundaries between subsequent results; it writes results to the output stream; it unmaps files from the file manager, when an EOF result is encountered. It can be initialized by calling the function `init_writer_thread`, and terminated by calling the function `free_writer_thread`. The thread terminates when it claims a `NULL` result.

The function `process_results` is the main loop of the thread. It uses function `claim_result` (see section Results queue) to claim a result, function `handle_boundary` to handle the boundary condition between subsequent results, function `write_to_output` to write a result to the output stream and function `unmap_next_file` (see section File manager) to unmap a file that has been processed. 

A boundary condition between consecutive results arises when consecutive chunks divide a substring of subsequent characters (for example, the first chunk ends in "bbaa", and the second chunk starts with "aaab", resulting to encodings "2b2a" and "3a1b"). If this condition is met, it is handled before writing the encoded data to the output stream by subtracting the last character count from the first chunk and adding it to the second chunk (first chunk becomes "2b" and the second chunk becomes "5a1b"). 

## Notes about the Implementation

- Dynamic memory management is used to minimize memory usage and simplify memory ownership. This comes with some caveats: especially the handover of data between threads might lead to memory leaks. To mitigate this, thread resources are stored in a way that the main thread retains access to them, so it can perform cleanup operations when the thread terminates. 

- Dynamic memory management is also slower than static memory management. Carefully implementing static memory management to some areas of the project could improve performance. 

