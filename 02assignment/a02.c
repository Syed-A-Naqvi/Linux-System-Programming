#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<limits.h>
#include<unistd.h>
#include<stdarg.h>


#define MAX_LINE_LENGTH 1024
#define PATH_MAX 4096
#define LOG_FILE "./logs.txt"

// will append a string describing an event to a log file
void logEvent(const char *format, ...) { 

    va_list args;

    // opening log file in append mode
    FILE* file = fopen(LOG_FILE, "a+");
    if(file == NULL){
        perror("Error opening log file.\n");
        return;
    }

    // initializing the args variable to point to the first of the variable arguments 
    va_start(args, format);
    // using the variable arguments and the passed format string to write to log file
    vfprintf(file, format, args);
    // cleaning up variable argument list
    va_end(args);

    // reinitializing args to point to the first of the variable arguments
    va_start(args, format);
    // using variable arguments and formatted string to output to the console
    vprintf(format, args);
    // cleaning up variable argument again
    va_end(args);

    // closing file
    fclose(file);

}

// will read file line-by-line and create a directory structure for each line
int makeDirs(char* fname){

    // pointer to the file to be read from
    FILE* file = fopen(fname, "r");

    // will store each line for processing
    char lineBuffer[MAX_LINE_LENGTH];

    // output error in the case a file could not be read
    if(file == NULL){
        // writing error log 
        logEvent("Error opening record file %s\n.", fname);
        return -1;
    }
    else{
        
        // logging successsful file open
        logEvent("Successfully opened record file %s\n.", fname);

        // store working dir path
        char workingdr[PATH_MAX];

        // creating the user and admin folders to which more users will be added
        if((mkdir("./user",0700) == -1) || (mkdir("./admin",0700) == -1)){
            logEvent("Error creating user or root directory.\n");
            return -1;
        }
        else {
            getcwd(workingdr, sizeof(workingdr));
            logEvent("Successfully created root directory in  %s\n.", fname);
            logEvent("Successfully created user directory in  %s\n.", fname);
        }

        while(fgets(lineBuffer, sizeof(lineBuffer), file)){
            // for (int i = 1; i < strlen(lineBuffer); i++)
            // {
            //     /* code */
            // }
            printf("line = %sstrlen(line) = %ld\n", lineBuffer, strlen(lineBuffer));
        }
    }
}

int main(int argc, char const *argv[])
{
    char recordFile[] = "./dirs.txt";
    makeDirs(recordFile);
    return 0;
}
