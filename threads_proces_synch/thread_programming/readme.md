pthread library is separate from the standard c library.
In order to compile threadCreation.c, which uses functions implemented in the pthread library, use the following:

`gcc threadCreation.c -o threadCreation -pthread`