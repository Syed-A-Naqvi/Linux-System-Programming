#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>


int main(int argc, char const *argv[])
{

    // ------------------------INITIALIZING SOCKET------------------------    

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        printf("Error creating socket.\n");
        return -1;
    }
    else
    {
        printf("Client socket initialized!\n\n");
    }

    // server endpoint
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);
    
    // connecting to server endpoint
    int connectionResult = connect(clientSocket, (struct sockaddr *)&svrAddress, sizeof(svrAddress));
    if (connectionResult < 0)
    {
        printf("Error establishing connection.\n");
        return -1;
    }
    else
    {
        printf("Connection established.\n");
        printf("Server IP: %s\n", inet_ntoa(svrAddress.sin_addr));
        printf("Server port: %d\n\n", ntohs(svrAddress.sin_port)); 
    }


    // ------------------------CLIENT-SERVER COMMUNICATION------------------------

    char RxBuffer[128] = {};
    char TxBuffer[128] = {};
    if (argv[1] != NULL)
    {
        strcpy(RxBuffer, argv[1]); // initial temperature command-line argument funcationality shown in lab output requirement
    }
    
    for(int i = 0; 1; i++)
    {
        if(RxBuffer[0] == '\0')
        {
            printf("Enter temperature in fahrenheit (enter \"stop\" to kill client and server): ");
            fgets(RxBuffer, sizeof(RxBuffer), stdin);
            RxBuffer[strcspn(RxBuffer,"\n")] = 0;
        }

        if(strcmp(RxBuffer, "stop") == 0){
            sprintf(TxBuffer, "%s", "stop");
            int bytesSent = send(clientSocket, TxBuffer, sizeof(TxBuffer), 0);
            printf("Kill command entered ... Terminating program ...\n");
            break;
        }

        printf("Sending temperature %s°F in Fahrenheit to server for conversion to Celsius...\n", RxBuffer);

        sprintf(TxBuffer, "%s", RxBuffer);
        int bytesSent = send(clientSocket, TxBuffer, sizeof(TxBuffer), 0);
        memset(RxBuffer, sizeof(RxBuffer), 0);

        int bytesRead = recv(clientSocket, RxBuffer, sizeof(RxBuffer), 0);
        printf("Temperature in Celsius recieved from server after conversion -> %s°C\n\n", RxBuffer);

        memset(RxBuffer, 0, sizeof(RxBuffer));
        memset(TxBuffer, 0, sizeof(TxBuffer));

    }

    return 0;
}
