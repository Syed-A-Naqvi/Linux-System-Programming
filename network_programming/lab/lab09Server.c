#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main(int argc, char const *argv[])
{
    // creating a TCP/IP socket for ipv4 communications
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        printf("Error creating socket.\n");
    }
    else
    {
        printf("Server socket created!\n");
    }

    // creating a socket address structure that will be bound
    // to the previously created socket
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET; // address family is ipv4
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // specific socket address set to loop back
    svrAddress.sin_port = htons(50000); // socket will use port 50000 (converted to network byte order)
    int lenSvrAddress = sizeof(svrAddress); // length of socket address structure

    // binding the newly created socket to the specified socket address structure
    int bindResult = bind(serverSocket, (struct sockaddr *)&svrAddress, lenSvrAddress);
    if (bindResult < -1)
    {
        printf("Error binding socket.\n");
    }
    else
    {
        printf("Socket bound successfully.\n");
    }
    
    
    // listening on the socket for client connection requests 
    int listenResult = listen(serverSocket, -1);
    if (listenResult < 0)
    {
        printf("Error listening to socket.\n");
    }
    else
    {
        printf("Listening on socket...\n\n");
    }
    
    
    struct sockaddr_in cltAddress;
    socklen_t lenCltAddress = sizeof(cltAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &cltAddress, &lenCltAddress);
    if (clientSocket < 0)
    {
        printf("Error accepting connection.\n");
    }
    else
    {
        printf("Accepted new connection!\n");
        printf("Client IP: %s\n", inet_ntoa(cltAddress.sin_addr));
        printf("Client port: %d\n", ntohs(cltAddress.sin_port));        
        printf("Connection established!\n\n");
    }

    char RxBuffer[128] = {};
    char TxBuffer[128] = "This mesage is being sent.";

    int bytesRead = recv(clientSocket, RxBuffer, sizeof(RxBuffer), 0);
    printf("Bytes recieved from client: %d\n", bytesRead);
    printf("Message recieved from client --> %s\n\n", RxBuffer);

    int bytesSent = send(clientSocket, TxBuffer, sizeof(TxBuffer), 0);
    printf("Bytes sent to client: %d\n", bytesSent);
    printf("Message sent to client --> %s\n", TxBuffer);



    return 0;
}
    