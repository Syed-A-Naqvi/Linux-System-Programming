#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main(int argc, char const *argv[])
{

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        printf("Error creating socket.\n");
    }
    else
    {
        printf("Client socket initialized!\n");
    }

    // server endpoint
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);
    
    // connecting to server endpoint
    int connectionResult = connect(clientSocket, (struct sockaddr *)&svrAddress, sizeof(svrAddress));
    if (connectionResult < 0)
    {
        printf("Error establishing connection.\n");
    }
    else
    {
        printf("Connection established.\n");
        printf("Server IP: %s\n", inet_ntoa(svrAddress.sin_addr));
        printf("Server port: %d\n", ntohs(svrAddress.sin_port)); 
    }

    char RxBuffer[128] = {};
    char TxBuffer[128] = "This is the client's message.";

    int bytesSent = send(clientSocket, TxBuffer, sizeof(TxBuffer), 0);
    printf("Bytes sent to server: %d\n", bytesSent);
    printf("Message sent to server --> %s\n", TxBuffer);  
    
    int bytesRead = recv(clientSocket, RxBuffer, sizeof(RxBuffer), 0);
    printf("Bytes recieved from server: %d\n", bytesRead);
    printf("Message recieved from server --> %s\n\n", RxBuffer);

    return 0;
}
