#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    
    char command[1024] = {};
    char service[128] = {"sys-bus-pci-drivers-nvidia.device"};
    snprintf(command, sizeof(command), "systemctl is-active %s", service);

    FILE *stream = popen(command, "r");

    char line[1024];

    if (fgets(line, sizeof(line), stream) != NULL)
    {
        line[strlen(line)-1] = 0;
        if (strcmp(line, "inactive") == 0)
        {
            printf("%s service inactive.\n", service);
        }
        else
        {
            printf("%s service active.\n", service);
        }
    }
    

    return 0;
}
