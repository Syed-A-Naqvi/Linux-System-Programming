#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>

int main(int argc, char const *argv[])
{

    int serverSocket;
    serverSocket = socket(AF_INET,SOCK_STREAM, 0);
    if (serverSocket == 0)
    {
        printf("Error creating server socket\n");
    }
    else
    {
        printf("Server socket created.\n");
    }

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = INADDR_ANY; // 127.0.0.1 loopback address or localhost
    svrAddress.sin_port = htons(27000); // 49152 - 65536 okay range*******
                                        // 0 - 1024 reserved range
                                        // 1024 - 49151 registered still okay ig

    int lenAddress = sizeof(svrAddress);
    int bindResult = bind(serverSocket, (struct sockaddr *) &svrAddress, lenAddress);
    if (bindResult < 0)
    {
        printf("Error binding socket.\n");
    }
    else
    {
        printf("Socket bound successfully.\n");
    }

    int listenResult = listen(serverSocket, 1);
    if (listenResult < 0)
    {
        printf("Error listening to server socket.\n");
    }
    else
    {
        printf("Listening to the socket successfully.\n");
    }
    
    int clientSocket = accept(serverSocket, (struct sockaddr *)&svrAddress, (socklen_t *)&lenAddress);
    if(clientSocket < 0){
        printf("Error accepting connections.\n");
    }
    else{
        printf("Accepting new connections.\n");
    }

    printf("Connection Established.\n");

    char RxBuffer[128] = {};
    char TxBuffer[128] = "Message acknowledged";

    int rcvdBytes = recv(clientSocket, RxBuffer, sizeof(RxBuffer), 0);
    printf("Recieved %d bytes from the client.\n",rcvdBytes);
    printf("Recieved message --> \"%s\".\n",RxBuffer);

    int sntBytes = send(clientSocket,TxBuffer, sizeof(TxBuffer), 0);
    printf("Acknowledgement sent to the client.\n");

    close(clientSocket);
    close(serverSocket);

    return 0;
}

