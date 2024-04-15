#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>
#include<netdb.h>
#include<pthread.h>
#include<string.h>


int main(int argc, char const *argv[])
{

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        printf("Unable to create client socket.\n");
        close(clientSocket);
        return -1;
    }
    else
    {
        printf("Client socket created successfully.\n");
    }

    int connectResult = connect(clientSocket, (struct sockaddr *) &svrAddress, sizeof(svrAddress));
    if (clientSocket == -1)
    {
        printf("Unable to connect to server.\n");
        close(clientSocket);
        return -1;
    }
    else
    {
        printf("Successfully connected to server.\n");
    }

    char name[200] = {};
    printf("Enter your name: ");
    if(fgets(name, sizeof(name), stdin) == NULL)
    {
        printf("Error reading from cmd.\n");
    }
    name[strlen(name)-1] = 0;

    char time_str[200] = {};
    time_t currtime = time(NULL);
    sprintf(time_str, "%s", ctime(&currtime));

    int sbytes = send(clientSocket,name, sizeof(name), 0);
    if(sbytes == -1){
        printf("Error sending name to server.\n");
    }

    // dummy recv
    int rbytes = recv(clientSocket, name, sizeof(name),0);

    sbytes = send(clientSocket, time_str, sizeof(time_str), 0);
    if(sbytes == -1){
        printf("Error sending time to server.\n");
    }

    printf("Ticket reserved.\n");

    return 0;
}


