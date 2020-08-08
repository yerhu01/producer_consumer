This extra uses POSIX semaphores rather than POSIX mutexes and condition variables (which were required for the assignment). 

In addition to this, it also implements a barrier in the add function (it requires at least 3 clients requesting to add a value before adding the values into the buffer).

All extra code is found in bbuffer.c
