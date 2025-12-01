#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

# define BUFFER_SIZE 1024

void errorHandling(char *message);

int main(int argc,char **argv)
{
    int serverSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize;
    int strLength;
    char message[BUFFER_SIZE];

    if(argc != 2)
    {
        printf("Usage : %s <port> ",argv[0]);
       exit(1);
    }

    //Create server socket
    serverSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if(serverSocket == -1)
        errorHandling("socket() error");
    
    //Initialize server socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(atoi(argv[1]));

    //Assign an IP address and port to the server socket
    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1)
        errorHandling("bind() error");

    //Accept the connection request from the first client in the waiting queue
    while(1)
    {
        clientAddressSize = sizeof(clientAddress);
        strLength=recvfrom(serverSocket, message, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddress, &clientAddressSize);
        sendto(serverSocket, message, strLength, 0, (struct sockaddr*)&clientAddress, clientAddressSize);

        printf("Message from client : %s", message);
    }

    //Close the socket
    close(serverSocket);
    
    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc(' ',stderr);
    exit(1);
}