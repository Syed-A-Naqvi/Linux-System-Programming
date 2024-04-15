#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    char *ip = "192.168.1.22";
    char command[1024] = {};
    char modify[2];

    printf("Enter 'a' to add rule and 'r' to remove rule: ");

    fgets(modify, sizeof(modify), stdin);

    if (strcmp(modify, "a") == 0)
    {
        sprintf(command, "sudo /sbin/iptables -I INPUT -s %s -j DROP", ip);
        system(command);
    }
    else
    {
        printf("Option treated as 'r'.\n");
        sprintf(command, "sudo /sbin/iptables -D INPUT -s %s -j DROP", ip);
        system(command);
    }

    sprintf(command, "sudo iptables -L -n");
    FILE *ptr = popen(command, "r");
    char line[1024] = {};
    
    printf("\nViewing ip tables:\n");
    while(fgets(line, sizeof(line), ptr) != NULL)
    {
        printf("%s",line);
    }

    return 0;
}

