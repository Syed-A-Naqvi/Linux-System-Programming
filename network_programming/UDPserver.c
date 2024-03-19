#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>

int main(int argc, char const *argv[])
{
    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1)
    {
        printf("Error creating socket.\n");
    }
    else
    {
        printf("Socket created.\n");
    }
    
    struct sockadd_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = INADDR_ANY; //127.0.0.1
    svrAddress.sin_port = htons(27000);
    

    return 0;
}


