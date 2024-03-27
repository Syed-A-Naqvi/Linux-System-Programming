#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>
#include<errno.h>


#define MAXTHREADS 1

// semaphore to control accurate threadCount updating
sem_t tc_semaphore;
sem_t mt_semaphore;

// a thread will decrement upon exiting
// main function will increment when creating new thread
int threadCount = 0;

// THREAD FUNCTION
void* manageClient(void* arg)
{
    // getting server-side client socket
    int clientSocket = *(int *)arg;

    // communicating with client
    char RxBuffer[1024] = {};
    char TxBuffer[1024] = {};

    while(1)
    {
        // recieving messages from client
        int rcvd = recv(clientSocket, RxBuffer, sizeof(RxBuffer),0);
        if (rcvd == -1)
        {
            printf("Error recieving message from client.\n");
        }
        else if (rcvd == 0)
        {
            printf("\nClient has been disconnected.\n\n");
            break;
        }
        else
        {
            RxBuffer[strcspn(RxBuffer, "\n")] = '\0';
            printf("Message recieved from client -> \"%s\"\n", RxBuffer);
        }

        // creating acknowledgment message
        sprintf(TxBuffer, "Acknowledging message: \"%s\"\n", RxBuffer);

        // sending message acknowledgement
        int sent = send(clientSocket, TxBuffer, strlen(TxBuffer)+1,0);
        if (sent == -1)
        {
            printf("Error sending message acknowledgement.\n");
        }
        else
        {
            printf("Acknowledgement sent.\n");
        }

        memset(RxBuffer, '\0', sizeof(RxBuffer));
        memset(TxBuffer, '\0', sizeof(TxBuffer));
        
    }
    
    close(clientSocket);

    sem_wait(&tc_semaphore);
    threadCount--;
    sem_post(&tc_semaphore);

    sem_post(&mt_semaphore);
    pthread_exit(NULL);

}

int main(int argc, char const *argv[])
{

    // initializing semaphores
    sem_init(&tc_semaphore,0,1);
    sem_init(&mt_semaphore,0,MAXTHREADS);

    // CREATING LISTENING SOCKET //
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket != -1)
    {
        printf("Socket created successfully.\n");
        int flags = fcntl(serverSocket, F_GETFL, 0);
        fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);
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
        printf("Successfully listening on socket...\n\n");
    }
    else
    {
        printf("Unable to listen on socket. Terminating...\n");        
        return -1;
    }

    pthread_t clientThreads[MAXTHREADS];

    while (1)
    {

        // give clients time to send requests
        sleep(5);

        // ACCEPTING INCOMING CONNECTION, CREATING NEW SOCKET AND STORING CLIENT-SIDE SOCKET ADDRESS //
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (clientSocket == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                if (threadCount == 0)
                {
                    break;
                }
            }
            else
            {
                printf("Unable to accept connection.\n");
            }
        }
        else
        {
            printf("Connection accepted!\n");
            printf("Client IP: %s\n", inet_ntoa(clientAddress.sin_addr));        
            printf("Client Port Number: %d\n\n", ntohs(clientAddress.sin_port));

            // creating thread to service client connection
            sem_wait(&mt_semaphore);
            if(pthread_create(&clientThreads[threadCount], NULL, manageClient, (void *)&clientSocket) == 0)
            {
                sem_wait(&tc_semaphore);
                threadCount++;
                sem_post(&tc_semaphore);
            }
            else            
            {
                printf("Error creating thread for client.\n");
            }

        }



    }
        
    close(serverSocket);

    return 0;
}
