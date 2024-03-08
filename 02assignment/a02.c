#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<stdarg.h>
#include<time.h>


#define PATH_MAX 4096
#define MAX_FILENAME 256
#define LOG_FILE "./logs.txt"

// appending events to a log file as well as displaying to console
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

// returns pointer to dynamically allocated memory containing source string
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
        closedir(dir);
        return 1;
    }
    else{
        return 0;
    }
    
}

// creating profile.txt for a path for usrnm, appending timestamp, setting permissions
int makeProfile(const char* path, const char* usrnm){
    
    // creating profile.txt for usrnm 
    FILE* profile = fopen(path, "w+");
    if(profile == NULL){
        logEvent("Error creating %s profile text file.\n", usrnm);
    }
    else{
        logEvent("%s profile text file created!\n", usrnm);
    }

    // getting current time 
    char timestamp[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%a %b %d %H:%M:%S %Y", t);

    // creating string to write to file
    int stringLength = sizeof(usrnm)+sizeof(" profile created on ")+sizeof(timestamp);
    char *info = malloc(stringLength);
    memset(info, 0, stringLength);
    strcat(info, usrnm);
    strcat(info, " profile created on ");
    strcat(info, timestamp);

    //writing string to profile.txt
    fprintf(profile, "%s\n", info);

    // closing file
    fclose(profile);

    // setting permissions to read-only for profile.txt for all users
    if(chmod(path, 0444) == 0){
        logEvent("Permission for file at %s set to read only!\n\n", path);
    }
    else{
        logEvent("Error setting permissions for file at %s.", path);
    }

    // freeing dynamic variable
    free(info);


}

// accepts a list of directories and index of the directory we are considering
int makeItems(const char **dirs, int idx){
    
    //dirs array legend:
        // 0 -> usertype
        // 1 -> username
        // 2 -> home
        // 3 -> profile

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
            logEvent("Company directory created!\n\n");
        }
    }

    // creating internal directory structure based on the current path
    if(!dirExists(path)){

        switch (idx){
            case 0:
                // creating either admin or user root directory
                if(mkdir(path, 0777) == 0){
                    if (strcmp(dirs[idx],"admin") == 0){
                        logEvent("admin root directory created!\n\n");                    
                    }
                    else{      
                        logEvent("user root directory created!\n\n");
                    }
                }
                break;

            case 1:
                // creating user directory
                if(mkdir(path, 0777) == 0){
                    logEvent("%s user directory created!\n", dirs[idx]);
                }
                break;

            case 2:
                // creating home directory for specified user
                if(mkdir(path, 0777) == 0){
                    logEvent("Home directory for %s created!\n", dirs[1]);
                }

                // creating path of profile.txt in home folder of current user
                char *newpath = realloc(path, strlen(path) + sizeof("/profile.txt"));
                if (newpath == NULL){
                    logEvent("Could not resize path to include /profile.txt.\n");
                }
                else{
                    path = newpath;
                    strcat(path,"/profile.txt");
                    makeProfile(path, dirs[1]);
                }                
                break;

            default:
                break;
        }
    }

    // freeing dynamically allocated variable
    free(path);
    
}



// MAIN FUNCTION
// RESULTS IN CALLS TO ALL OTHER FUNCTIONS
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
        
        // capable of storing the longest possible name for a file
        char pathBuffer[PATH_MAX];
        char fileName[MAX_FILENAME];

        logEvent("Reading file and creating directories ...\n\n");

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
            
            // following loop uses the number of commas encountered to extract items of current path
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
            // creating all items part of the directory structure defined by current line read
            for (size_t i = 0; i < 3; i++){
                // will make each directory defined by first three comma-seperated values
                // automatically creates profile.txt after creating home dir for a user
                makeItems((const char **)path, i);
            }
            
            // freeing all dynamically allocated strings storing items extracted from current line
            for (int i = 0; i < 4; i++){
                free(path[i]);
            }
            
        }

        // closing file after all directory structures created
        fclose(file);

    }

}

int main(int argc, char const *argv[])
{
    // this is the path of the file that contains directory structures
    char recordFile[] = "./dirs.txt";

    // creating directory structures
    createDirectories(recordFile);

    return 0;
}
