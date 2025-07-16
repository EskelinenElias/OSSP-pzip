# The Parallel Zip Tool 

The parallel zip (pzip) tool compresses one or more input files using run length encoding. It uses parallel processing to increase compression speed. 

## Usage

The tool can be used to run length encode one or more files to a single output file. If multiple files are encoded into one output file, file boundary information can't be recovered from the encoded file. The tool can be run with the command `./pzip file1 [file2 ...]`. The output can be directed to another file (here, `output_file`) by changing the command to `./pzip file1 [file2 ...] > output_file`. 

## Project structure

The project is structured as follows: 
- Source code can be found in the directory `src` and the corresponding header files in the directory `include`. 
- Compiled code goes in the `obj`-directory. 
- Tests can be found from the `tests`-directory. Automated tests can be run from the root directory by running the command `./run_tests.sh`. 
- Project can be compiled by running the command `make` in the root directory. This compiles the project according to the instructions in `Makefile` in the root directory.

## Implementation 

The algorithm utilizes parallel processing to speed up the encoding process. The main thread spawns worker threads and a specialized writer thread. The number of worker threads is determined from the number of cores available. 

The main thread starts processing the input files one by one. An input file is mapped into memory and then the main thread divides the data in the file to 'tasks'. A task is a chunk of the input data of a certain length (`TASK_SIZE` in `include/constants.h`). The tasks are put into a task queue, from where the worker threads can claim the task. 

Worker threads then start to process these tasks, and yield the results to a results queue. The writer thread claims these results from the results queue (in the correct order) and writes them to the output file. 

Race conditions are avoided by using a mutex locks. Dividing the input data into tasks means that work is distributed dynamically. Memory management is also done dynamically to minimize memory usage and to simplify memory ownership between threads. Busy waiting is avoided by using condition variables. 

The separate writer thread enables writing the results to output as they are processed. As the process is mainly IO-bound, this should speed it up. 
 
## The main thread

The main function parses and validates the input arguments, initializes all needed components (file manager, tasks queue, worker threads, results queue, and the writer thread), yields tasks to the tasks queue and then cleans up the process. 

## The file manager

The file manager handles memory mapping the input files. It ensures, that already mapped files are not mapped again and that files are not unmapped too early. This is achieved by using a mapped file queue, where new files are added to the end of the queue, and files are unmapped from the front of the queue. New files are compared to the files in the queue to avoid trying to map them again. File manager also ensures, that all opened files are closed when the program exits.

## The task structure

The task structure contains a pointer to the input data (a memory mapped file), the size of the input data to process and a reserved index to the results queue, which is used when yielding the result of the task to the results queue. 

## The tasks queue

The tasks queue manages encoding tasks and makes sure they are claimed in the correct order and that no task is claimed twice. The main thread yields tasks to the tasks queue, and the worker threads claim tasks from the tasks queue. 

## The worker threads

The worker threads process the tasks in the tasks queue and then yield the results to the results queue, to the reserved index which is part of the task. The worker claims a task from the tasks queue, encodes the data, and yields the result to the results queue. The thread will terminate upon error or when it claims a NULL task.

## The result data structure

The result data structure represents a chunk of encoded data. It contains a character array, a character counts array and the current size of these arrays. 

## The results queue

The results queue manages results (encoded data) and makes sure they remain in the correct order. Worker threads yield results to the results queue, and the writer thread claims these results in order. In order to yield a result to the results queue, an index for the result must be reserved first; this is done when initializing the corresponding task. The result is then yielded with the reserved index, and the result is put in it's reserved spot in the queue. This is done to ensure that the writer thread receives the results in the correct order. 

## Writer thread

The writer thread has multiple jobs: it claims encoding results from the results queue; it handles boundaries between subsequent results; it writes results to the output stream; it unmaps files from the file manager, when an EOF result is encountered. It can be initialized by calling the function `init_writer_thread`, and terminated by calling the function `free_writer_thread`. The thread terminates when it claims a `NULL` result.

The function `process_results` is the main loop of the thread. It uses function `claim_result` (see section Results queue) to claim a result, function `handle_boundary` to handle the boundary condition between subsequent results, function `write_to_output` to write a result to the output stream and function `unmap_next_file` (see section File manager) to unmap a file that has been processed. 

A boundary condition between consecutive results arises when consecutive chunks divide a substring of subsequent characters (for example, the first chunk ends in "bbaa", and the second chunk starts with "aaab", resulting to encodings "2b2a" and "3a1b"). If this condition is met, it is handled before writing the encoded data to the output stream by subtracting the last character count from the first chunk and adding it to the second chunk (first chunk becomes "2b" and the second chunk becomes "5a1b"). 

## File manager

The file manager manages the input files. It maps the input files to memory, keeps track of mapped files, and unmaps files when they are no longer needed. It also provides a mechanism to unmap the next file when an EOF result is encountered. The most important job of the file manager is to make sure that no attempt is made to map a file to memory more than once, as this would lead to undefined behavior. 

The file manager is implemented as a `file_manager_t` structure. It can be initialized by calling the `init_file_manager` function, and freed using the `free_file_manager` function. The structure stores an array of mapped files and a mutex to ensure thread safety. Mapped files are stored in the order they are added, in a queue; this way, they can also be unmapped in the same order.

Mapped files are stored as `mapped_file_t` structures. These can be initialized (mapped) by calling the `init_mapped_file` function, and freed using the `free_mapped_file` function. The structure stores the file descriptor, the file size, and a pointer to the mapped memory and file stats to uniquely identify the file (the inode number and device ID).

The function `map_next_file` takes a `file_manager_t` pointer and a filepath. It checks that the file exists, is accessible and is not already mapped; if the file is already mapped, the function returns a pointer to the already created `mapped_file_t` structure. The file is also moved to the end of the queue to delay unmapping, and the original index is set to `NULL` (this indicates to the function `unmap_next_file` that no file needs to be unmapped). If the file is not mapped, it is mapped and added to the end of the queue.

The function `unmap_next_file` takes a `file_manager_t` pointer and returns an integer (return code). The function checks if the next spot in the mapped file queue (pointer by the `head` index) contains a mapped file. If it does, the file is unmapped and removed from the queue. If the queue is empty, the function simply returns. 
