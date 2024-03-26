#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<time.h>

int main(int argc, char const *argv[])
{

    // CREATING CLIENT-SIDE ENPOINT/SOCKET //
    int connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectionSocket == -1)
    {
        printf("Error creating socket. Terminating client...\n");
        return -1;
    }
    else
    {
        printf("Socket created successfully.\n");
    }


    // SERVER SOCKET ADDRESS //
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);
    socklen_t lenAddress = sizeof(svrAddress);    
    

    // CONNECTING TO SERVER //
    int connectionResult = connect(connectionSocket, (struct sockaddr *)&svrAddress, lenAddress);
    if (connectionResult == -1)
    {
        printf("Error connecting to server. Terminating client...\n");
        return -1;
    }
    else
    {
        printf("Successfully connected to server.\n");
        printf("IP: %s\n", inet_ntoa(svrAddress.sin_addr));
        printf("Port Number: %d\n\n", ntohs(svrAddress.sin_port));
    }


    // COMMUNICATING WITH SERVER //    
    char RxBuffer[128] = {};
    char TxBuffer[128] = {};

    for (int i = 0; i < 11; i++)
    {   

        sleep(1);
        sprintf(TxBuffer, "(client 1) message %d", i);
        int bytesSNT = send(connectionSocket, TxBuffer, strlen(TxBuffer)+1, 0);
        if (bytesSNT == -1)
        {
            printf("Error sending message %d to server.\n", i);
        }
        else
        {
            printf("Message sent to server -> \"%s\"\n", TxBuffer);
        }

        int bytesRCVD = recv(connectionSocket, RxBuffer, sizeof(RxBuffer),0);
        if (bytesRCVD == -1)
        {
            printf("Error recieving message %d acknowledgement from server.\n", i);
        }
        else
        {
            printf("%s\n\n", RxBuffer);
        }        

        memset(RxBuffer, '\0', sizeof(RxBuffer));
        memset(TxBuffer, '\0', sizeof(TxBuffer));
        
    }

    close(connectionSocket);

    return 0;
}
