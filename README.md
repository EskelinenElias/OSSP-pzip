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

## The task data structure

The task data structure contains a pointer to the input data (a memory mapped file), the size of the input data to process and a reserved index to the results queue, which is used when yielding the result of the task to the results queue. 

## The tasks queue

The tasks queue manages encoding tasks and makes sure they are claimed in the correct order and that no task is claimed twice. The main thread yields tasks to the tasks queue, and the worker threads claim tasks from the tasks queue. 

## The worker threads

The worker threads process the tasks in the tasks queue and then yield the results to the results queue, to the reserved index which is part of the task data. The worker claims a task from the tasks queue, encodes the data, and yields the result to the results queue. The thread will terminate upon error or when it claims a NULL task.

## The result data structure

The result data structure represents a chunk of encoded data. It contains a character array, a character counts array and the current size of these arrays. 

## The results queue

The results queue manages results (encoded data) and makes sure they remain in the correct order. Worker threads yield results to the results queue, and the writer thread claims these results in order. In order to yield a result to the results queue, an index for the result must be reserved first; this is done when initializing the corresponding task. The result is then yielded with the reserved index, and the result is put in it's reserved spot in the queue. This is done to ensure that the writer thread receives the results in the correct order. 

## The writer thread

The writer thread claims encoding results from the results queue. The results queue ensures that the results are in the correct order. The writer thread handles boundaries between subsequent results and then writes the results to the output stream. The writer thread will terminate upon error or when it claims a NULL result. When encountering an EOF result (encoding data with the size 0), the writer thread tells the file manager to unmap the next file. 

A boundary condition can arise where subsequent chunks divide a substring of subsequent characters (for example, the first chunk end in "bbaa", and the second chunk starts with "aaab", resulting to encodings "2b2a" and "3a1b"). If this condition is met, it is handled before writing the encoded data to the output stream by substracting the last character count from the first chunk and adding it to the second chunk (first chunk becomes "2b" and the second chunk becomes "5a1b"). 