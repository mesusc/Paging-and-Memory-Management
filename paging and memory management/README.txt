Assumptions:
1. priority is based on run command for LRU(least recently ran will be first to popped out from lru and those doesnt ran wont bother lru)
2. if it's not possible to swapin the run command then it wont run
3. Swapin error in sense the swapped out processes from mainmemory cannot be swapped into virtual memory
4. File size =0,still gets a pid
5. file not found won't get a pid
6. even after the error msg ,a success msg will be printed, but it shows ignore the success msg
7. If nothing is present in lru then a random process is swapped out
8. add,sub should have 3 arguments and every other command should have exact number of arguments as given in question
9.for swapin first swapout happens and then only swapping in.
10.for swapout and kill it clears the mainmemory and it wont copy the loaded or add or sub data to virtual memory
11. No academic dishonesty 

PS:I'll explain the code clearly to TA during demo,explaining completely here is a bit difficult.
