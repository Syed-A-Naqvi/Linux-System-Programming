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

int main(int argc, char const *argv[])
{
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        printf("Error creating server socket.\n");
    }
    else
    {
        printf("Socket created successfully.\n");
    }

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = INADDR_ANY;
    svrAddress.sin_port = htons(8080);
    int lenSvrAddress = sizeof(svrAddress);

    int bindResult = bind(serverSocket, (struct sockaddr *)&svrAddress, lenSvrAddress);
    if (bindResult == -1)
    {
        printf("Error binding socket.\n");
    }
    else
    {
        printf("Socket bound successfully.\n");
    }

    int listenResult = listen(serverSocket, 10);
    if (listenResult == -1)
    {
        printf("Error listening on server socket.\n");
    }
    else
    {
        printf("Listening on server socket...\n\n");
    }
    
    struct sockaddr_in clientAddress;
    socklen_t lenClientAddress = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &lenClientAddress);
    if (clientSocket == -1)
    {
        printf("Error accepting client connection.\n");
    }
    else
    {
        printf("New client connection accepted.\n");
        printf("Connection established.\n");
        printf("Client IP: %s\n", inet_ntoa(clientAddress.sin_addr));        
        printf("Client Port Number: %d\n\n", ntohs(clientAddress.sin_port));
    }

    char RxBuffer[1024] = {};
    char TxBuffer[1024] = {};

    int bytesRcvd;
    do {
        bytesRcvd = recv(clientSocket, RxBuffer, sizeof(RxBuffer) - 1, 0);
        if (bytesRcvd == -1) {
            perror("Error receiving from client");
            break;
        } else if (bytesRcvd == 0) {
            printf("Client closed connection\n");
            break;
        } else {
            RxBuffer[bytesRcvd] = '\0';  // Null-terminate the string
            printf("%s", RxBuffer);
        }
    } while (strstr(RxBuffer, "\r\n\r\n") == NULL);


    // time_t timer = time(NULL);
    // char * time_str = ctime(&timer);
    // char html[128] = {};
    // sprintf(html, "<html><body><h1>%s</h1></body></html>", time_str);
    // sprintf(TxBuffer, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %lu\r\n\r\n%s\n", strlen(html), html);
    // int bytesSnt = send(clientSocket, TxBuffer, strlen(TxBuffer), 0);
    // if (bytesSnt == -1) {
    //     perror("Error sending response to client");
    // } else {
    //     printf("Sent from server: %s\n", TxBuffer);
    // }

    // READING FILE CONTENTS //
    // WE CAN ALSO OPEN A JPG IMAGE INSTEAD OF AN HTML FILE, THE FOLLOWING CODE SHOULD WORK IN EITHER CASE
    FILE * fptr = fopen("./index.html", "r"); // opening and returning a pointer to the begining of an html file
    fseek(fptr, 0, SEEK_END); // file position indicator moved to end of file
    long file_size = ftell(fptr); // returns number of bytes from beginning of file to current indicator position
    rewind(fptr); // move position indicator back to start of file
    char *html_TxBuffer = malloc(file_size + 1); // allocating enough space to store contents of html file
    fread(html_TxBuffer, 1, file_size, fptr); // reading file_size elements each of which being 1 byte
    html_TxBuffer[file_size] = '\0'; // setting last character of html_TxBuffer to null terminator 
    fclose(fptr); // closing file


    // CREATING HTML RESPONSE HEADER //
    char html_header[128] = {};
    sprintf(html_header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %lu\r\n\r\n", file_size);

    send(clientSocket, html_header, strlen(html_header) + 1,0);
    send(clientSocket, html_TxBuffer, file_size+1,0);

    printf("HTML page sent to client.\n");

    close(clientSocket);
    close(serverSocket);
    free(html_TxBuffer);

    
    return 0;
}
