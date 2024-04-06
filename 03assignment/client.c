#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

#define MAX_ATTEMPTS 5

int main(int argc, char const *argv[])
{

// -----------------------------------------SETUP---------------------------------------//\

    // SERVER SOCKET ADDRESS INFORMATION //
    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddress.sin_port = htons(50000);
    socklen_t svrAddrLen = sizeof(svrAddress);

    // CREATING CONNECTION SOCKET //
    int connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectionSocket == -1)
    {
        printf("Error creating connection socket.\n");
        printf("Terminating client...\n");
        return -1;
    }
    printf("Client socket initialized!\n");

    //CONNECTING TO SERVER //
    int connectionResult = connect(connectionSocket, (struct sockaddr *)&svrAddress, svrAddrLen);
    if (connectionResult == -1)
    {
        printf("Error connecting to server.\n");
        printf("Terminating client...\n");
        return -1;
    }

    // TRANSMISSION AND RECEPTION BUFFERS
    char tx[100] = {};
    char rx[1024] = {};

// -----------------------------------------SETUP---------------------------------------//



// -------------------------------------COMMUNICATION-----------------------------------//

    // RECEIVING WELCOME MESSAGE FROM SERVER //
    int rbytes = recv(connectionSocket, rx, sizeof(rx), 0);
    {
        if (rbytes == -1)
        {
            printf("Error receiving welcome message from server.\n");
            printf("Can not gurantee connection. Terminating client...\n");
            close(connectionSocket);
            return 0;
        }
        else if (rbytes == 0)
        {
            printf("Server has shutdown.\n");
            close(connectionSocket);
            return 0;
        }
        else
        {
            printf("%s\n", rx);
        }
        
    }

    // PROMPTING USER FOR NAME
    int inAttempts = 0;
    while (1)
    {   

        if(inAttempts == MAX_ATTEMPTS){
            printf("%d unnsuccessful attempts. Terminating client....\n\n", inAttempts);
            close(connectionSocket);
            return -1;
        }

        printf("\nPlease enter your name: ");
        if (fgets(tx, 100*sizeof(char), stdin) == NULL)
        {
            inAttempts++;
            printf("Nothing received form the command line, please try again. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
        }
        else if (tx[0] == '\n')
        {
            inAttempts++;
            printf("You just pressed enter without providing a name. Please try again. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
        }
        else
        {   
            tx[strlen(tx)-1] == 0;
            break;
        }

    }

    // SENDING USER NAME
    int sbytes = send(connectionSocket, tx, sizeof(tx),0);
    {
        if (sbytes == -1)
        {
            printf("Error sending user name to server. Terminating client ...\n");
            close(connectionSocket);
            return -1;
        }
        else
        {
            printf("Connection established.\n\n");
        }
    }
    
    while (1)
    {  

        // RECEIVING MENU FROM SERVER
        // rbytes = recv(connectionSocket, rx, sizeof(rx), 0);
        // {
        //     if (rbytes == -1)
        //     {
        //         printf("Error receiving menu from server.\n");
        //         printf("Can not gurantee connection. Terminating client...\n");
        //         close(connectionSocket);
        //         return 0;
        //     }
        //     else if (rbytes == 0)
        //     {
        //         printf("Server has closed connection.\n");
        //         close(connectionSocket);
        //         return 0;
        //     }
        //     else
        //     {
        //         printf("%s\n", rx);
        //     }

        // } 

        // PROMPTING USER TO ENTER CHOICE
        inAttempts = 0;
        while (1)
        {   

            if(inAttempts == MAX_ATTEMPTS){
                printf("%d unnsuccessful attempts. Terminating client....\n\n", inAttempts);
                close(connectionSocket);
                return -1;
            }

            printf("\nPlease enter your choice: ");
            if (fgets(tx, 100*sizeof(char), stdin) == NULL)
            {
                inAttempts++;
                printf("Nothing received form the command line, please try again. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
            }
            else
            {
                if (tx[1] == '\n')
                {
                    tx[strlen(tx)-1] = 0;
                }
                else
                {
                    inAttempts++;
                    printf("Please enter a valid choice. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
                    continue;
                }
            }

            int validChoice = 0;
            char choices[] = {'1', '2', '3', '4', '5', '6'};
            for (int i = 0; i < 6; i++)
            {
                if(tx[0] == choices[i]){
                    validChoice = 1;
                    break;
                }
            }
            if (validChoice == 1)
            {
                break;
            }
            else
            {
                inAttempts++;
                printf("Please enter a valid choice. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
            }

        }        

        // SENDING CHOICE BACK TO SERVER
        sbytes = send(connectionSocket, tx, sizeof(tx),0);
        if (sbytes == -1)
        {
            printf("Error sending choice to server. Terminating client ...\n");
            close(connectionSocket);
            return -1;
        }

        // RECEIVING BENFIT INFORMATION FROM SERVER
        // rbytes = recv(connectionSocket, rx, sizeof(rx), 0);
        // {
        //     if (rbytes == -1)
        //     {
        //         printf("Error receiving benefit information from server.\n");
        //         printf("Can not gurantee connection. Terminating client...\n");
        //         close(connectionSocket);
        //         return 0;
        //     }
        //     else if (rbytes == 0)
        //     {
        //         printf("Server has shutdown.\n");
        //         close(connectionSocket);
        //         return 0;
        //     }
        //     else
        //     {
        //         printf("%s\n", rx);
        //     }
        // }

        // ASKING USER IF THEY NEED MORE INFORMATION
        inAttempts = 0;
        while (1)
        {   

            if(inAttempts == MAX_ATTEMPTS){
                printf("%d unnsuccessful attempts. Terminating client....\n\n", inAttempts);
                close(connectionSocket);
                return -1;
            }

            printf("\nDo you need any more information? (Yes = y, No = n): ");
            if (fgets(tx, 100*sizeof(char), stdin) == NULL)
            {
                inAttempts++;
                printf("Nothing received from the command line, please try again. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
                continue;
            }
            if (strlen(tx) > 2)
            {
                inAttempts++;                    
                printf("Not a valid option, please try again. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
            }
            else
            {
                tx[strlen(tx)-1] = 0;

                if( (tx[0] == 'y') || (tx[0] == 'n') ){
                    break;
                }
                else
                {
                    inAttempts++;                    
                    printf("Not a valid option, please try again. %d attempts remaining.\n", MAX_ATTEMPTS - inAttempts);
                }
            }

        }   

        // SENDING RESPONSE TO SERVER AND TERMINATING IF RESPONSE WAS 'n'
        sbytes = send(connectionSocket, tx, sizeof(tx),0);
        if (sbytes == -1)
        {
            printf("Error sending choice to server. Terminating client ...\n");
            close(connectionSocket);
            return -1;
        }

        if(tx[0] == 'n'){
            close(connectionSocket);
            printf("Thank you for consulting Benefits Canada. Have a good day!\n");
            return 0;
        }


    }
    
// -------------------------------------COMMUNICATION-----------------------------------//


    return 0;
}