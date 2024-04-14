#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<arpa/inet.h>
#include<time.h>

int main(int argc, char const *argv[])
{

    // SSL SETUP --------------------------------------------------------------------------------------
    printf("Initializing SSL\n");
    SSL_library_init();
    printf("Loading Error Settings\n");
    SSL_load_error_strings();
    printf("Loading Algorithms\n");
    OpenSSL_add_all_algorithms();

    SSL_CTX *ctx;

    printf("Setting SSL Context\n");
    ctx = SSL_CTX_new(TLS_client_method());

    if(!ctx){
        fprintf(stderr, "Error generating SSL context.\n");
        ERR_print_errors_fp(stderr);
    }

    printf("Verifying Certificate\n");
    if (SSL_CTX_load_verify_locations(ctx, "./certificate.pem", NULL) <= 0)
    {
        fprintf(stderr, "Error loading list of trusted certificates.\n");
        ERR_print_errors_fp(stderr);
    }
    
    // SOCKET SETUP --------------------------------------------------------------------------------------
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        printf("Error creating socket.\n");
    }
    else
    {
        printf("Socket created successfully.\n");
    }

    struct sockaddr_in svrAddress;
    svrAddress.sin_family = AF_INET;
    inet_aton("127.0.0.1", &svrAddress.sin_addr);
    svrAddress.sin_port = htons(50000);

    int connectionResult = connect(clientSocket, (struct sockaddr *) &svrAddress, sizeof(svrAddress));
    if(connectionResult == -1){
        printf("Error connecting to server.\n");
    }
    else
    {
        printf("Successfully connected to server.\n");
    }
    printf("Connection Established.\n");

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientSocket);

    if (SSL_connect(ssl) <= 0)
    {
        fprintf(stderr, "Error during TLS handshake with server.\n");
        ERR_print_errors_fp(stderr);
    }
    else
    {
        printf("Successfully completed TLS handshake.\n");
    }


    char TxBuffer[1024] = {"Hello from the client."};
    int sbytes = SSL_write(ssl, TxBuffer, sizeof(TxBuffer));
    if (sbytes < 0)
    {
        printf("Error sending bytes.\n");
    }
    else if (sbytes == 0)
    {
        printf("Server Shutdown normally.\n");
    }
    else
    {
        printf("Message Sent.\n");
    }
    
    int shdstatus = SSL_shutdown(ssl);
    if(shdstatus == 0){
        SSL_shutdown(ssl);
    }

    close(clientSocket);

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    

    return 0;
}
