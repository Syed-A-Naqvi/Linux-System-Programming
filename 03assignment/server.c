#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>

struct Client {
    struct sockaddr_in cltAddress;
    socklen_t cltAddrLen;
    int cltSocket;
};

void* manageClient(void* arg){
    return 0;
}

int main(int argc, char const *argv[])
{
    
    // SERVER LISTENING SOCKET ADDRESS STRUCTURE //
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = INADDR_ANY;
    svrAddress.sin_port = htons(50000);
    socklen_t svrAddrLen = sizeof(svrAddress);

    // CREATING LISTENING SOCKET //
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    {
        if (serverSocket == -1)
        {
            printf("Error creating server listening socket.\n");
            printf("Terminating server...\n");
            return -1;
        }
        else
        {
            printf("Server listening socket created successfully.\n");
        }
    }

    // BINDING LISTENING SOCKET //
    int bindResult = bind(serverSocket, (struct sockaddr *)&svrAddress, svrAddrLen);
    {
        if (bindResult == -1)
        {
            printf("Error binding server listening socket.\n");
            printf("Terminating server...\n");
            return -1;
        }
        else
        {
            printf("Server listening socket bound successfully.\n");
        }    
    }

    // LISTENING ON LISTENING SOCKET //
    int listenResult = listen(serverSocket, 1);
    {
  
    } 

    // PERPETUALLY ACCEPTING CONNECTIONS AND ASSIGNING THREADS //
    while(1)
    {
        // storing client information
        struct Client * clientPtr = malloc(sizeof(struct Client));
        // accepting incomming connection
        int acceptResult = accept(serverSocket, (struct sockaddr *)&clientPtr->cltAddress, &clientPtr->cltAddrLen);
        if (acceptResult == -1)
        {
            // acknowledging connection error and moves on to next iteration
            printf("Error accepting connection.\n");
            continue;
        }
        else
        {
            // displaying successfull connection message and client information
            printf("Connection Established.\n");
            printf("Client IP: %s.\n", inet_ntoa(clientPtr->cltAddress.sin_addr));
            printf("Client Port: %d.\n", ntohs(clientPtr->cltAddress.sin_port));

            // creating new child thread to service client
            pthread_t tid;
            pthread_create(&tid, NULL, manageClient, clientPtr);
            pthread_detach(tid);
        } 

    }

    close(serverSocket);
    
    return 0;
}


