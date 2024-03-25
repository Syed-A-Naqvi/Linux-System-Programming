#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char const *argv[])
{
    // CREATING SERVER CONNECTION SOCKET //
    int connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    return 0;
}
