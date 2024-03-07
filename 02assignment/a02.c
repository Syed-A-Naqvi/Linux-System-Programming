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

// returns pointer to allocated memory in which source string is copied
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

// returns 1 if directory at path exists, 0 else
int dirExists(const char *path){
    DIR *dir = opendir(path);
    if (dir){
        logEvent("directory at path %s already exists.\n", path);
        return 1;
    }
    else{
        logEvent("directory at path %s does not exist.\n", path);
        return 0;
    }
    
}

// accepts array of directories part of a path and the index up to which the path needs to be created
int makeDirs(const char **dirs, int idx){
    
    // getting length of path
    int pathLen = 0;
    char company[] = "./SiliconConsulting";
    pathLen += strlen(company) + 1;
    for (int i = 0; i <= idx; i++){
        pathLen += strlen(dirs[i]) + 1;
    }
    pathLen++;
    
    // creating path string
    char *path = malloc(pathLen);
    memset(path, 0, pathLen);
    strcat(path, company);
    for (int i = 0; i <= idx; i++){
        strcat(path, "/");
        strcat(path, dirs[i]);
    }
    
    // creating company parent folder
    if(!dirExists(company)){
        if(mkdir(company, 0777) == 0){
            logEvent("Company directory created!\n");
        }
    }

    // creating internal directory structure based on the current path
    if(!dirExists(path)){

        switch (idx){
            case 0:
                if(mkdir(path, 0777) == 0){
                    if (strcmp(dirs[idx],"admin") == 0){
                        logEvent("admin root directory created!\n");                    
                    }
                    else{      
                        logEvent("user root directory created!\n");
                    }
                }
                break;

            case 1:
                if(mkdir(path, 0777) == 0){
                    logEvent("%s user directory created!\n", dirs[idx]);
                }
                break;

            case 2:
                if(mkdir(path, 0777) == 0){
                    logEvent("Home directory for %s created!\n", dirs[1]);
                }
                break;

            default:
                break;
        }
    }
    
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

        // extracting item names from each line and then creating all items line-by-line
        while (fgets(pathBuffer, PATH_MAX, file) != NULL){
            
            int idx = 0;
            int comma = 0;

            //path array legend:
                // 0 -> usertype
                // 1 -> username
                // 2 -> home
                // 3 -> profile                
            char *path[4];
            
            for(int i = 0; (pathBuffer[i] != '\n') && (pathBuffer[i] != '\0'); i++){
                
                char c = pathBuffer[i];

                if(c != ','){
                    fileName[idx] = c;
                    idx++;
                }
                else{
                    comma++;
                    fileName[idx] = '\0';
                    
                    path[comma-1] = allocateAndCopy(fileName);

                    memset(fileName,0, MAX_FILENAME);
                    idx = 0;                     
                }

            }
            // creating directory structure
            for (size_t i = 0; i < 3; i++){
                makeDirs((const char **)path, i);
            }
            

            for (int i = 0; i < 4; i++){
                free(path[i]);
            }
            
        }

    }

}

int main(int argc, char const *argv[])
{
    char recordFile[] = "./dirs.txt";
    createDirectories(recordFile);
    return 0;
}
