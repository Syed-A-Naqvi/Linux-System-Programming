#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>


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
    
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(27000);
    
    int lenAddress = sizeof(svrAddress);

    int bindResult = bind(serverSocket, (struct sockaddr *) &svrAddress, lenAddress);
    if (bindResult < 0)
    {
        printf("Error binding to the socekt.\n");
    }
    else
    {
        printf("Socket binding successful.\n");
    }


    // will store the information recieved from client in this buffer
    char RxBuffer[128] = {};

    //infromation about the client will be stored here
    struct sockaddr_in client; //client endpoint information
    int lenClient = sizeof(client); //length of client endpoint information

    while (1)
    {
        printf("Waiting ...\n\n");

        // recvfrom will store the information of the connecting client in the provided struct
        int receivedBytes = recvfrom(serverSocket,RxBuffer, sizeof(RxBuffer), 0, (struct sockaddr *) &client,&lenClient);
        if(receivedBytes)
        {   

            if (strcmp(RxBuffer, "end") == 0)
            {
                printf("Client IP: %s\n", inet_ntoa(client.sin_addr));
                printf("Client Port: %d\n", ntohs(client.sin_port));            
                printf("Received from the client --> \"%s\"\n", RxBuffer);
                printf("Terminating server...goodbye :D\n");
                break;
            }
            

            printf("Client IP: %s\n", inet_ntoa(client.sin_addr));
            printf("Client Port: %d\n", ntohs(client.sin_port));            
            printf("Received from the client --> \"%s\"\n\n", RxBuffer);
        }

        memset(RxBuffer, 0, sizeof(RxBuffer));
        memset(&client, 0, sizeof(client));

    }

    close(serverSocket);
    
    return 0;
}


