#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>
#include<netdb.h>
#include<pthread.h>

#define MAXTICKETS 3

typedef struct
{
    char name[1024];
    char purchase_time[1024];
} Ticket;

sem_t sem;
Ticket tickets[MAXTICKETS] = {};
int ticketCount = 0;

void* reserve_ticket(void* arg)
{
    Ticket *temp = malloc(sizeof(Ticket));
    int client = *((int*)arg);
    char rxBuffer[1024] = {};

    int rbytes = recv(client, rxBuffer, sizeof(rxBuffer), 0);
    if (rbytes == 0)
    {
        printf("Client has terminated. closing thread.\n");
        close(client);
        free(temp);
        return (void *)0;
    }
    else if (rbytes == -1)
    {
        printf("Problem receiving customer name. closing thread.\n");
        close(client);
        free(temp);
        return (void *)-1;
    }
    sprintf(temp->name, "%s", rxBuffer);

    printf("header: %s\n", temp->name);

    // dummy send
    int sbytes = send(client, rxBuffer, sizeof(rxBuffer),0);

    rbytes = recv(client, rxBuffer, sizeof(rxBuffer), 0);
    if (rbytes == 0)
    {
        printf("Client has terminated. closing thread.\n");
        close(client);
        free(temp);
        return (void *)0;
    }
    else if (rbytes == -1)
    {
        printf("Problem receving time of ticket purchase. closing thread.\n");
        close(client);
        free(temp);
        return (void *)-1;
    }
    sprintf(temp->purchase_time, "%s", rxBuffer);

    sem_wait(&sem);
    if(ticketCount < MAXTICKETS)
    {
        tickets[ticketCount] = *temp;
        ticketCount++;
    }
    sem_post(&sem);

    close(client);
    free(temp);
    return (void *) 0;
    
}

int main(int argc, char const *argv[])
{

    sem_init(&sem, 0, 1);

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        printf("Unable to create server socket.\n");
        close(serverSocket);
        return -1;
    }
    else
    {
        printf("Server socket created successfully.\n");
    }

    int bindResult = bind(serverSocket, (struct sockaddr *) &svrAddress, sizeof(svrAddress));
    if (bindResult == -1)
    {
        printf("Unable to bind server socket.\n");
        close(serverSocket);
        return -1;
    }
    else
    {
        printf("Server socket bound successfully.\n");
    }

    int listenResult = listen(serverSocket, 1);
    if (listenResult == -1)
    {
        printf("Unable to listen to server socket.\n");
        close(serverSocket);
        return -1;
    }
    else
    {
        printf("Listening on server socket...\n");
    }

    while(ticketCount < MAXTICKETS)
    {
        struct sockaddr_in client;
        socklen_t clientLen = sizeof(client);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&client, &clientLen);
        if (clientSocket == -1)
        {
            printf("Could not accept connaction.\n");
            close(clientSocket);
            continue;
        }
        else
        {
            pthread_t tid;
            pthread_create(&tid, NULL, reserve_ticket, (void *) &clientSocket);
            pthread_detach(tid);
        }
    }

    close(serverSocket);

    for (int i = 0; i < MAXTICKETS; i++)
    {
        printf("Ticket %d:\n", i);
        printf("Name = %s\n", tickets[i].name);
        printf("Purchase Time = %s\n\n", tickets[i].purchase_time);
    }

    return 0;
}


