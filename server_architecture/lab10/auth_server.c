#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>

struct Login
{
    char username[8];
    char password[8];
};

void* authenticate(void* arg){
    
    // login credentials
    struct Login logins[5] = {
        {"john", "12345"},
        {"steve", "canada"},
        {"bill", "ontario"},
        {"elon", "tesla"},
        {"tim", "apple"}        
    };

    // retrieving connection socket
    int connectionSocket = *(int *)arg;

    // will store received credentials
    char credentials[20] = {};

    // receiving password
    int rcvdBytes = recv(connectionSocket, credentials, sizeof(credentials), 0);
    if (rcvdBytes == -1)
    {
        printf("Error receiving credentials.\n");
        return NULL;
    }
    else if (rcvdBytes == 0)
    {
        printf("Client closed connection.\n");
        return NULL;
    }

    char * username = strtok(credentials, ",");
    char * password = strtok(NULL, ",");

    printf("Client requesting authentication...\n");

    // authentication response
    char authentication[30] = "Login Unsuccessful!";

    for (int i = 0; i < 5; i++)
    {
        if(strcmp(username, logins[i].username) == 0)
        {
            if (strcmp(password, logins[i].password) == 0)
            {
                sprintf(authentication, "Login Successful!");
            }
            
        }
    }

    int sntBytes = send(connectionSocket, authentication, sizeof(authentication), 0);
    if (sntBytes == -1)
    {
        printf("Error sending authentication response.\n");
        return NULL;
    }
    else
    {
        printf("Authentication response sent...\n\n");
    }

    close(connectionSocket);
    pthread_exit(NULL);

}

int main(int argc, char const *argv[])
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        printf("Error creating listening socket. Terminating server...\n");
        return -1;
    }
    else
    {
        printf("Server socket created succesfully.\n");
    }

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);

    int bindResult = bind(serverSocket, (struct sockaddr *)&svrAddress, sizeof(svrAddress));
    if (serverSocket == -1)
    {
        printf("Error binding socket. Terminating server...\n");
        return -1;
    }
    else
    {
        printf("Server bound successfully.\n");
    }

    int listenResult = listen(serverSocket, 1);
    if (listenResult == -1)
    {
        printf("Error listening on socket. Terminating server...\n");
        return -1;
    }
    else
    {
        printf("Listening on socket...\n\n");
    }

    while (1)
    {
        
        printf("Accepting new connections!\n");

        struct sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (clientSocket == -1)
        {
            printf("Error accepting connection.\n");
            continue;
        }
        else
        {
            printf("Connection established with %s\n", inet_ntoa(clientAddress.sin_addr));
            pthread_t tid;
            pthread_create(&tid, NULL, authenticate, (void *)&clientSocket);
            pthread_join(tid, NULL);
        }
        
    }

    close(serverSocket);
    
    return 0;
}



