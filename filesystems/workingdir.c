#include<stdio.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    char working_directory[FILENAME_MAX];
    getcwd(working_directory,sizeof(working_directory));

    return 0;
}
