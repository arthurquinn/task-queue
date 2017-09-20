## TASK QUEUE ## 

The task queue is deisgned to be simple and easy to use. Simply push a task with a priority onto the queue and once it is this tasks turn, it will run, and return a value to the task queue via a UDP socket. Once it is returned more tasks can be dispatched.

### Dispatcher ###

It is the dispatchers job to handle task execution. Whereas the primary purpose of the queue is to maintain the order in which tasks arrived, the dispatchers sole purpose is to make sure only a certain number of tasks are running at once (defined in code as 4). Once a task that the dispatcher sent out for execution returns to the dispatcher, the dispatcher will mark the tasks progress as complete or failed and pull the next task from a the priority queue.

### Recoverable Queue ###

This queue was designed with the intention that its state can be restored after a program crash. As items are enqueued into this queue they will be written out to a binary .qi file. As they are dequeued these files will be removed. The queue is initialized with a maximum size and a reconstruct directory. All queue items belonging to a queue will be written out and read from the specified reconstruct directory. 

The queue item will save its priority and raw data in the queue item save file. The data can be saved and retrieved using a void pointer and the length of the data pointed to in bytes. As such complex data objects cannot be stored in the queue item file. However it is ideal for storing a flat struct of non-reference values.

When a queue is initialized it will look in its reconstruct directory and load all queue item files found there. As the queue is enqueued and dequeued it will save and remove queue item files as necessary. When the program is interrupted, all of the queue items that were in the queue and their priorities/data are still saved in the reconstruct directory. The next time a queue is initialized with the same reconstruct directory, it will be reconstructed with all of the queue items that were present in the queue at the time of interruption maintaining priority order.

### Build Instructions ###

To make the library run `make` in the root directory

You can then include the recoverable queue in your project by including the headers located in the include/ directory and linking the static library in the lib/ directory.

To unit test the library run `make test`

### Documentation ###

Reference the header files for usage, documentation section is not yet complete.