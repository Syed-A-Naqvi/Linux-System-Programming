#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>


int main(int argc, char const *argv[])
{
    // CREATING LISTENING SOCKET //
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket != -1)
    {
        printf("Socket created successfully.\n");
    }
    else
    {
        printf("Could not create socket. Terminating...\n");
        return -1;
    }

    // CREATING SOCKET ADDRESS STRUCTURE //
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);
    socklen_t lenAddress = sizeof(svrAddress);

    // BINDING ADDRESS STRUCTURE TO SOCKET //
    int bindResult = bind(serverSocket, (struct sockaddr *)&svrAddress, lenAddress);
    if (bindResult == 0)
    {
        printf("Socket bound successfully to address structure.\n");
    }
    else
    {
        printf("Unable to bind socket to address structure. Terminating...\n");        
        return -1;
    }

    // LISTENING ON SOCKET FOR CONNECTION REQUESTS //
    int listenResult = listen(serverSocket, 2);
    if (listenResult == 0)
    {
        printf("Successfully listening on socket...\n");
    }
    else
    {
        printf("Unable to listen on socket. Terminating...\n");        
        return -1;
    }

    // ACCEPTING INCOMING CONNECTION, CREATING NEW SOCKET AND STORING CLIENT-SIDE SOCKET ADDRESS //
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int connectionSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddress);
    if (connectionSocket == -1)
    {
        printf("Unable to accept connection.\n");        
    }
    else
    {
        printf("Connection accepted!\n");        
    }


    // COMMUNICATING WITH CLIENT //
    char RxBuffer[128] = {};
    char TxBuffer[128] = {};

    while (1)
    {
        // RECIEVING FROM CLIENT //
        int bytesRCVD = recv(connectionSocket, RxBuffer, sizeof(RxBuffer), 0);
        if (bytesRCVD == -1)
        {
            printf("Error recieving message from client.\n");
        }
        else
        {   
            RxBuffer[strcspn(RxBuffer, '\n')] = '\0';
            printf("Message of %d bytes recieved from client -> \"%s\"\n", bytesRCVD, RxBuffer);
        }

        // CONSTRUCTING TRANSMISSION MESSAGE //
        sprintf(TxBuffer, "Acknowledgement of message: \"%s\"", RxBuffer);

        // SENDING TO CLIENT //
        int bytesSNT = recv(connectionSocket, TxBuffer, sizeof(TxBuffer), 0);
        if (bytesSNT == -1)
        {
            printf("Error sending acknowledgement to client.\n");
        }
        else
        {
            printf("Sending %d byte acknowledgement to client -> \"%s\"\n", bytesSNT, TxBuffer);
        }

        if(strcmp(RxBuffer,"stop") == 0){
            printf("Kill command recieved. Terminating server...\n");
            break;
        }

        memset(RxBuffer, '\0', sizeof(RxBuffer));
        memset(TxBuffer, '\0', sizeof(TxBuffer));

    }
        
    close(serverSocket);
    close(connectionSocket);

    return 0;
}
