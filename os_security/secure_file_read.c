#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen("./shadow", "r");

    if (file == NULL)
    {
        printf("There was a problem opening the file. errno = %s\n", strerror(errno));

    }
    else
    {
        printf("The file was successfully opened.\n");
    }
    
    
    return 0;
}
