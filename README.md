# ECEN5813_SP19_Project1
Project 1: Interactive memory manipulation
Functions created: 
Help - The initial greeting on startup must minimally indicate how to invoke the help function. 
Invoking the help function will minimally list, to standard output, the available commands and how to invoke them.

Allocate memory - Uses malloc() to allocate a block of memory for use by the application. 
The user specifies the number of bytes or words of memory to allocate.

Free memory - Uses free() to release the previously allocated block.

Display memory words - Display contents of memory in terms of 32-bit words in hexadecimal format. 
The user may specify an address and the number of words to display.

Write memory words - User specifies an address and the 32-bit value to write. 
Memory at the indicated location is modified accordingly.

Invert block - Use an XOR operation to invert all memory bits in a specified area of memory. 
On completion, this command should report the amount of time taken to perform the operation.

