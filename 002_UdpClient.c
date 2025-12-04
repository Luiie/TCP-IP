#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

# define BUFFER_SIZE 1024

void errorHandling(char *message);

int main(int argc, char **argv)
{
    int clientSocket;
    struct sockaddr_in serverAddress, fromAddress;
    char message[BUFFER_SIZE];
    int strLength = 0;
    socklen_t addressSize;

    if(argc != 3)
    {
        printf("usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // Create client socket
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if(clientSocket == -1)
        errorHandling("socket() error");

    //Initialize server socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));

    //connect UDP
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    //Receive data byte-by-byte with boundary check
    while(1)
    {
        fputs("Input message: ", stdout);
        fgets(message, sizeof(message), stdin);

        if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break;
        
        write(clientSocket, message, strlen(message));
        addressSize = sizeof(fromAddress);
        strLength = read(clientSocket, message, sizeof(message));
        message[strLength] = 0;

        printf("Message from server : %s", message);
    }

    //Close the socket
    close(clientSocket);

    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}