# The Parallel Zip Tool 

The parallel zip (pzip) tool compresses one or more input files using run length encoding. It uses parallel processing to increase compression speed. 

The algorithm works as follows: 

1. Validate inputs
2. Read input files to a combined buffer
3. Run-length encode the data in the buffer in chunks
4. Handle boundaries between subsequent chunks and write the chunks to output stream
5. Free allocated memory. 

The parallel processing is done by dividing the input data to chunks of the same size. The chunks are then fed into a task queue, from where worker threads will claim them and process them. Race conditions are avoided by using a mutex lock in the queue. 

The worker threads encode the data to encoded data objects, which contain an array for characters and an array for subsequent character counts, as well as capacity and current size parameters. 

A boundary condition can arise where subsequent chunks divide a substring of subsequent characters (for example, the first chunk end in "bbaa", and the second chunk starts with "aaab", resulting to encodings "2b2a" and "3a1b"). If this condition is met, it is handled before writing the encoded data to the output stream by substracting the last character count from the first chunk and adding it to the second chunk (first chunk becomes "2b" and the second chunk becomes "5a1b"). 

## The main function

The main function parses the input arguments, and 

## The task queue

This implementation of `pzip` utilizes a task queue to distribute encoding tasks to the threads. The input is divided into $n$ chunks, and for each chunk, an encoding task is added to the task queue. An encoding task consists of the chunk of the input, the length of the chunk and a pointer to an encoded data structure for the output. The threads then claim these encoding tasks, process them and claim more when they are ready. The queue utilizes mutex locks to avoid race conditions on claiming tasks. More tasks are added to the queue as the already added tasks are processed; the length of the task queue is by default twice the number of threads. After all tasks have been processed, the threads are terminated. 

## The encoding task structure

The encoding task structure contains a pointer to a character array, the length of the array to be processed, and a pointer to an encoded data structure, where the encoding results will be stored. 

## The worker 

Each thread runs an endless loop, which consists of the following steps: 
1. The thread checks if the task queue has available tasks, waits if it doesn't
2. As a task becomes available, the thread claims the task. 
3. The thread checks, if the task is empty, and terminates if it is - an empty task can be used to terminate the thread. 
4. The thread processes the input data in the task, and stores the encoded data in the output slot of the task. 
5. The thread repeats steps 1-4. 

## The encoded data structure

The encoded data structure consists of a character array for storing the characters, and an integer array for storing the counts of subsequent matching characters, as well as the current utilied length of the arrays and the allocated. capacity of the arrays.

## The encoding loop  

The encoding function takes as arguments a pointer to an input character array, the length of the array, and a pointer to a output encoded data structure. The algorithm then loops trough the input array, counting subsequent matching characters and storing the counts and characters to the output structure. 




