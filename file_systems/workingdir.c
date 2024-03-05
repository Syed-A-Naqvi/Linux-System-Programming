#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>


int main(int argc, char const *argv[])
{
    //FILENAME_MAX gives the local max filename size
    char workingDir[FILENAME_MAX];

    getcwd(workingDir, sizeof(workingDir));
    printf("%s\n", workingDir);

    struct dirent* entry;
    DIR *dir = opendir(workingDir);
    while ((entry = readdir(dir)) != NULL){
        
        if(!(strncmp(entry->d_name, ".", strlen(".")) == 0) && (entry->d_type == DT_REG)){
            printf("%s\n",entry->d_name);
        }

    }
    closedir(dir);

    return 0;
}
