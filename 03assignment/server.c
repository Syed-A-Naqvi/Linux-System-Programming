#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#include<stdarg.h>
#include<time.h>


#define LOG_FILE "./benefit_logs.txt"


struct Client {
    struct sockaddr_in cltAddress;
    socklen_t cltAddrLen;
    int cltSocket;
};

// APPENDING EVENTS TO LOG_FILE
void logEvent(const char *format, ...) { 

    va_list args;

    // opening log file in append mode
    FILE* file = fopen(LOG_FILE, "a+");
    if(file == NULL){
        perror("Error opening log file.\n");
        return;
    }

    // appending timestamp to log file
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char timestamp[50];
    strftime(timestamp, sizeof(timestamp), "%a %b %d %H:%M:%S %Y", local);
    fprintf(file, "%s\n", timestamp);

    // initializing the args variable to point to the first of the variable arguments 
    va_start(args, format);
    // using the variable arguments and the passed format string to write to log file
    vfprintf(file, format, args);
    // cleaning up variable argument list
    va_end(args);


    // closing file
    fclose(file);

}

// THREAD FUNCTION
void* manageClient(void* arg){

    // OBTAINING CLIENT INFORMATION
    struct Client *client = (struct Client *)arg;

    // TRANSMISSION AND RECEPTION BUFFERS
    char menu[1024] = {};
    char tx[1024] = {};
    char rx[1024] = {};

    // SENDING WELCOME MESSAGE TO CLIENT
    sprintf(tx, "%s","Welcome to Benefits Canada Server!");
    int sbyts = send(client->cltSocket, tx, strlen(tx)+1,0);
    if (sbyts == -1)
    {
        printf("Error sending welcome message to client.\n");
        printf("Cannot gurantee client connection. Closing Socket...\n");
        close(client->cltSocket);
        free(client);
        return (void*)1;
    }
    else
    {
        logEvent("Welcome message sent!\n");
    }
    

    // RECEIVING CLIENT NAME
    int rbytes = recv(client->cltSocket, rx, sizeof(rx), 0);
    {
        if (rbytes == -1)
        {
            printf("Error receiving client name.\n");
            printf("Cannot gurantee connection. Closing socket...\n");
            close(client->cltSocket);
            free(client);
            return (void*)1;
        }
        else if (rbytes == 0)
        {
            printf("Client has closed connection.\n");
            close(client->cltSocket);
            free(client);
            return (void*)1;
        }
        
    }

    // CONSTRUCTING PERSONALIZED MENU
    char *username = strdup(rx);
    sprintf(menu, "Hello %s\n", username);
    free(username);
    strncat(menu, "To get information about benefits, please select from the followng:\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "1. Canada Workers Benefits\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "2. Canada Child Benefits\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "3. Registered Disability Savings Plan\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "4. War Veterans Allowance\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "5. Disability Tax Credit\n", sizeof(menu) - strlen(menu) - 1);
    strncat(menu, "6. Canada Dental Benefits", sizeof(menu) - strlen(menu) - 1);

    // EXTRACTING AND STORING ALL BENEFITS DATA FROM FILE
    FILE *file = fopen("./Benefits_Canada.txt", "r");
    char* benefits[6] = {};
    for (int i = 0; i < 6; i++)
    {
        if(fgets(rx, sizeof(rx), file) == NULL){
            printf("Error reading from benefits file. Closing client socket...\n");
            close(client->cltSocket);
            free(client);
            return (void*)1;
        }
        rx[strlen(rx)-1] = 0;
        benefits[i] = malloc(strlen(rx)+1);
        sprintf(benefits[i], "%s", rx);
    }
    fclose(file);    


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
            return (void*)-1;
        }
        else
        {
            logEvent("Menu sent!\n");
        }
        
    
        // RECEIVING CLIENT BENEFITS OPTION
        rbytes = recv(client->cltSocket, rx, sizeof(rx), 0);
        {
            if (rbytes == -1)
            {
                printf("Error receiving benefits option from client.\n");
                printf("Cannot gurantee connection. Closing socket...\n");
                close(client->cltSocket);
                free(client);
                return (void*)-1;
            }
            else if (rbytes == 0)
            {
                printf("Client has closed connection.\n");
                printf("Closing socket.\n");
                close(client->cltSocket);
                free(client);
                return (void*)0;
            }           
        }
        int option = atoi(rx);

        // CREATING STRING FOR REQUESTED BENEFIT
            char *benefit = strdup(benefits[option - 1]);
            char *remainder;
            char *token = __strtok_r(benefit, ";", &remainder);

            // skipping the serial
            token = __strtok_r(NULL, ";", &remainder);

            // benefit code
            char *code = strdup(token);
            token = __strtok_r(NULL, ";", &remainder);
            logEvent("%s information requested!\n", code);

            // benefit name
            char *name = strdup(token);
            token = __strtok_r(NULL, ";", &remainder);

            // benefit description
            char *description = strdup(token);
            token = __strtok_r(NULL, ";", &remainder);

            // benefit URL
            char *URL = strdup(token);
            token = __strtok_r(NULL, ";", &remainder);

            // creating string
            sprintf(tx,
                    "Please find below information about %s\n\n%s (%s):\n%s\n\nPlease visit the following link for more information:\n%s",
                    name, name, code, description, URL);

            // freeing all dynamic arrays
            free(benefit);
            free(code);
            free(name);
            free(description);
            free(URL);

        // SENDING BENEFIT INFORMATION TO CLIENT
        sbyts = send(client->cltSocket, tx, strlen(tx)+1,0);
        {
            if (sbyts == -1)
            {
                printf("Error sending menu to client.\n");
                printf("Cannot gurantee client connection. Closing Socket...\n");
                close(client->cltSocket);
                free(client);
                return (void*)-1;
            }
            else{
                printf("Requested information sent!\n");
                logEvent("Requested information sent!\n");
            }
        }

        // RECEIVING CLIENT RESPONSE FOR MORE INFORMATION, TERMINATING THREAD IF RESPONSE IS 'n'
        rbytes = recv(client->cltSocket, rx, sizeof(rx), 0);
        {
            if (rbytes == -1)
            {
                printf("Error receiving request for more information from client.\n");
                printf("Cannot gurantee connection. Closing socket...\n");
                close(client->cltSocket);
                free(client);
                return (void*)-1;
            }
            else if (rbytes == 0)
            {
                printf("Client has closed connection.\n");
                printf("Closing socket.\n");
                close(client->cltSocket);
                free(client);
                return (void*)0;
            }           
        }        

        if(rx[0] == 'n'){
            break;
        }

    }
    
    // FREEING DYNAMICALLY ALLOCATED SPACE AND CLOSING ACTIVE CLIENT SOCKET
    close(client->cltSocket);
    free(client);

    return (void*)0;
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


