#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    int sock;
    struct sockaddr_in svrAddress;

    svrAddress.sin_family = AF_INET;
    inet_aton("127.0.0.1", &svrAddress.sin_addr);
 
    for (int i = start; i < end+1; i++)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1)
        {
            printf("Error creating socket.\n");
        }

        svrAddress.sin_family = AF_INET;
        inet_aton("127.0.0.1",&svrAddress.sin_addr);
        svrAddress.sin_port = htons(i);

        if (connect(sock, (struct sockaddr *)&svrAddress, sizeof(svrAddress)) == 0)
        {
            printf("Port number %d is open.\n", i);
        }
        close(sock);
        
    }


    return 0;
}
