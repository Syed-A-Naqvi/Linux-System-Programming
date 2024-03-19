#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == 0)
    {
        printf("Error creating socket.\n");
    }
    else
    {
        printf("Socket created successfully.\n");
    }

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_port = htons(27000);
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int connectResult = connect(clientSocket, (struct sockaddr *)& svrAddress, sizeof(svrAddress));
    if (connectResult < 0)
    {
        printf("Error establishing connection.\n");
    }
    else
    {
        printf("Connection established successfully.\n");
    }

    char TxBuffer[] = "Hello from Client";
    int sntBytes = send(clientSocket, TxBuffer, sizeof(TxBuffer),0);
    printf("Message of %d bytes sent to server.\n", sntBytes);

    char RxBuffer[128] = {};
    int rcvdBytes = recv(clientSocket, RxBuffer, sizeof(RxBuffer), 0);
    printf("Recieved message --> \"%s\" from server.\n",RxBuffer);


    close(clientSocket);

    return 0;
}
