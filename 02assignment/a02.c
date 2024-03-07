#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<stdarg.h>

#define PATH_MAX 4096
#define MAX_FILENAME 256
#define LOG_FILE "./logs.txt"

// will append a string describing an event to a log file as well as display to console
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

char* allocateAndCopy(const char *source){
    char *dest = malloc(sizeof(char)*strlen(source)+1);
    if(dest == NULL){
        logEvent("Malloc for string '%s' failed.\n", source);
    }
    else{
        strcpy(dest, source);
    }
    return dest;
}

// will read file line-by-line and create a directory structure for each line
int createDirectories(char* fname){

    logEvent("Opening directory structure file at path: %s ...\n", fname);
    FILE *file = fopen(fname, "r");
    
    if(file == NULL){
        logEvent("Error opening file. Aborting...\n");
        return -1;
    }
    else{
        logEvent("Successfully opened file.\n");
        logEvent("Reading file ...\n");
        
        // capable of storing the longest possible name for a file
        char pathBuffer[PATH_MAX];
        char fileName[MAX_FILENAME];

        while (fgets(pathBuffer, PATH_MAX, file) != NULL){
            
            int fIndx = 0;
            int comma = 0;
            char *usertype, *username, *home, *profile;
            
            for(int i = 0; (pathBuffer[i] != '\n') && (pathBuffer[i] != '\0'); i++){
                
                char c = pathBuffer[i];

                if(c != ','){
                    fileName[fIndx] = c;
                    fIndx++;
                }
                else{
                    comma++;
                    fileName[fIndx] = '\0';

                    switch (comma){
                        case 1:
                            usertype = allocateAndCopy(fileName);
                            break;
                        case 2:
                            username = allocateAndCopy(fileName);
                            break;
                        case 3:
                            home = allocateAndCopy(fileName);
                            break;
                        case 4:
                            profile = allocateAndCopy(fileName);   
                            break;
                        default:
                            break;
                    }

                    memset(fileName,0, MAX_FILENAME);
                    fIndx = 0;                     
                }

            }
            
            logEvent("usertype = %s, username = %s, home = %s, profile = %s\n",usertype,username,home,profile);
            free(usertype);
            free(username);
            free(home);
            free(profile);
        }

    }

}

int main(int argc, char const *argv[])
{
    char recordFile[] = "./dirs.txt";
    createDirectories(recordFile);
    return 0;
}
