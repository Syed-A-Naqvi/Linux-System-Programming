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

    // OBTAINING CLIENT INFORMATION //
    struct Client *client = (struct Client *)arg;

    // TRANSMISSION AND RECEPTION BUFFERS
    char menu[1024] = {};
    char tx[1024] = {};
    char rx[1024] = {};

    // SENDING WELCOME MESSAGE TO CLIENT //
    sprintf(tx, "%s","Welcome to Benefits Canada Server!");
    int sbyts = send(client->cltSocket, tx, strlen(tx)+1,0);
    if (sbyts == -1)
    {
        printf("Error sending welcome message to client.\n");
        printf("Cannot gurantee client connection. Closing Socket...\n");
        close(client->cltSocket);
        free(client);
        return -1;
    }

    // RECEIVING CLIENT NAME AND CREATING MENU
    int rbytes = recv(client->cltSocket, rx, sizeof(rx), 0);
    {
        if (rbytes == -1)
        {
            printf("Error receiving client name.\n");
            printf("Cannot gurantee connection. Closing socket...\n");
            close(client->cltSocket);
            free(client);
            return -1;
        }
        else if (rbytes == 0)
        {
            printf("Client has closed connection.\n");
            close(client->cltSocket);
            free(client);
            return 0;
        }
        
    }
    sprintf(menu, "%s\n", rx);
    strncat(menu, "To get information about benefits, please select from the followng:\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "1. Canada Workers Benefits\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "2. Canada Child Benefits\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "3. Registered Disability Savings Plan\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "4. War Veterans Allowance\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "5. Disability Tax Credit\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "6. Canada Dental Benefits", sizeof(menu) - strlen(menu) - 1);
    
    // PROVIDING INFORMATION TO CLIENT AS LONG AS REQUIRED
    while (1)
    {    
        // SENDING MENU TO CLIENT
        sbyts = send(client->cltSocket, menu, strlen(menu)+1,0);
        if (sbyts == -1)
        {
            printf("Error sending menu to client.\n");
            printf("Cannot gurantee client connection. Closing Socket...\n");
            close(client->cltSocket);
            free(client);
            return -1;
        }
    
        // RECEIVING CLIENT BENEFITS OPTION
        int rbytes = recv(client->cltSocket, rx, sizeof(rx), 0);
        {
            if (rbytes == -1)
            {
                printf("Error receiving benefits option from client.\n");
                printf("Cannot gurantee connection. Closing socket...\n");
                close(client->cltSocket);
                free(client);
                return -1;
            }            
        }

        
    
    
    
    
    
    }
    
    // FREEING DYNAMICALLY ALLOCATED SPACE AND CLOSING ACTIVE CLIENT SOCKET
    close(client->cltSocket);
    free(client);

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
    if (serverSocket == -1)
    {
        printf("Error creating server listening socket.\n");
        printf("Terminating server...\n");
        return -1;
    }

    // BINDING LISTENING SOCKET //
    int bindResult = bind(serverSocket, (struct sockaddr *)&svrAddress, svrAddrLen);
    if (bindResult == -1)
    {
        printf("Error binding server listening socket.\n");
        printf("Terminating server...\n");
        return -1;
    }

    // LISTENING ON LISTENING SOCKET //
    int listenResult = listen(serverSocket, 1);
    if (listenResult == -1)
    {
        printf("Error listening on socket.\n");
        printf("Terminating server...\n");
        return -1;
    }
    

    printf("Listening on socket for incoming connections...\n");


    // PERPETUALLY ACCEPTING CONNECTIONS AND ASSIGNING THREADS //
    while(1)
    {
        // storing client information
        struct Client * clientPtr = malloc(sizeof(struct Client));
        // accepting incomming connection
        clientPtr->cltSocket = accept(serverSocket, (struct sockaddr *)&clientPtr->cltAddress, &clientPtr->cltAddrLen);
        if (clientPtr->cltSocket == -1)
        {
            // acknowledging connection error and moving on to next iteration
            printf("Error accepting connection.\n\n");
            continue;
        }
        
        // creating new child thread to service client
        pthread_t tid;
        int threadCreation = pthread_create(&tid, NULL, manageClient, (void *)clientPtr);
        if (threadCreation != 0)
        {
            printf("Error creating thread for client communication.\n");
            printf("Dropping client connection.\n\n");
            close(clientPtr->cltSocket);
            free(clientPtr);
            continue;
        }
        
        // detatching thread so its resources are freed automatically upon completion without explicit join call
        pthread_detach(tid);

    }

    // CLOSING LISTENING SOCKET //
    close(serverSocket);
    
    return 0;
}


