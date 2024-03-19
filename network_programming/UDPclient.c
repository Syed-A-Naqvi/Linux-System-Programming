#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<time.h>



bool isInteger(const char* str)
{
    if (*str == '\0')
    {
        return false;
    }

    while (*str != '\0')
    {
        if(isdigit(*str)){
            str++;
        }
        else{
            return false;
        }
    }

    return true;
}

int main(int argc, char const *argv[])
{
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0)
    {
        printf("Error creating socket.\n");
    }
    else
    {
        printf("Successfully created socket.\n");
    }

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(27000);
        
    char TxBuffer[128] = {};

    while (1)
    {
        printf("Waiting ... \n\n");

        fgets(TxBuffer, sizeof(TxBuffer), stdin);
        TxBuffer[strcspn(TxBuffer,"\n")] = 0;

        if(isInteger(TxBuffer))
        {
            srand(time(NULL));
            int numIters = atoi(TxBuffer);
            for (int i = 0; i < numIters; i++)
            {
                sprintf(TxBuffer, "%d: random number %d.", i, rand());
                TxBuffer[strcspn(TxBuffer,"\n")] = 0;
                int sent = sendto(clientSocket, TxBuffer, sizeof(TxBuffer), 0,(struct sockaddr *)&svrAddress, sizeof(svrAddress));
                if (sent)
                {
                    printf("Message sent to the server --> \"%s\"\n", TxBuffer);
                    sleep(1);
                }
                memset(TxBuffer, 0, sizeof(TxBuffer));
            }
            printf("\n");
        }
        else
        {
            int sent = sendto(clientSocket, TxBuffer, sizeof(TxBuffer), 0,(struct sockaddr *)&svrAddress, sizeof(svrAddress));
            if (sent)
            {
                if(strcmp(TxBuffer, "end") == 0)
                {
                    printf("Message sent to the server: %s\n", TxBuffer);
                    printf("Terminating client...goodbye :D\n");
                    break;
                }

                printf("Message sent to the server: %s\n\n", TxBuffer);
            }
            memset(TxBuffer, 0, sizeof(TxBuffer));
        }
        
    }
    
    close(clientSocket);

    return 0;
}
