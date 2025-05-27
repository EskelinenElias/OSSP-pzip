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

