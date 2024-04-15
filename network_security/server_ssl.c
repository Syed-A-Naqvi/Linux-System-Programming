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
    ctx = SSL_CTX_new(TLS_server_method());

    if(!ctx){
        fprintf(stderr, "Error generating SSL context.\n");
        ERR_print_errors_fp(stderr);
    }

    printf("Loading Certificate\n");
    if (SSL_CTX_use_certificate_file(ctx, "./certificate.pem", SSL_FILETYPE_PEM) <= 0)
    {
        fprintf(stderr, "Error loading certifiacte.\n");
        ERR_print_errors_fp(stderr);
    }
    printf("Loading Key\n");
    if (SSL_CTX_use_PrivateKey_file(ctx, "./private_key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        fprintf(stderr, "Error loading private key.\n");
        ERR_print_errors_fp(stderr);
    }
    
    // SOCKET SETUP --------------------------------------------------------------------------------------
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
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

    if(bind(serverSocket, (struct sockaddr *) &svrAddress, sizeof(svrAddress)) == -1){
        printf("Error binding socket.\n");
    }
    else
    {
        printf("Socket bound successfully.\n");
    }

    if(listen(serverSocket, 1) == -1)
    {
        printf("Error listening to socket.\n");
    }
    else
    {
        printf("Listening to socket.\n");
    }

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLen);
    if(clientSocket == -1){
        printf("Error accepting new client connection.\n");
    }
    else
    {
        printf("Accepted new client connection.\n");
    }
    printf("Connection Established.\n");

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientSocket);

    if (SSL_accept(ssl) <= 0)
    {
        fprintf(stderr, "Error during TLS handshake with client.\n");
        ERR_print_errors_fp(stderr);
    }
    else
    {
        printf("Successfully completed TLS handshake with client.\n");
    }

    char RxBuffer[1024];
    int rbytes = SSL_read(ssl, RxBuffer, sizeof(RxBuffer));
    printf("Message: %s, # of bytes: %d\n", RxBuffer, rbytes);

    int shdstatus = SSL_shutdown(ssl);
    if(shdstatus == 0){
        SSL_shutdown(ssl);
    }
    close(clientSocket);
    close(serverSocket);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}
