The idea is to iterate over each process and check to see if the process has been completed.
If the process has not been completed, iterate over each reasource and see if there are enough instances
available to satisfy the amount required by that process to complete.
If at any point the process requires more resource instances than what is available, move on to next process.
if there are sufficient instances avaialable to complete the process, mark it as complete, add it to the sequence of process execution and return all allocated resources to the os (update available array).

Iterate over the list over and over until all processes have been completed.