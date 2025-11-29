#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void errorHandling(char *message);

int main(int argc,char **argv)
{
    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int clientAddressSize;
    char message[]="Hello HANY!";

    if(argc =! 2)
    {
        printf("Usage : %s <port> ",argv[0]);
       exit(1);
    }

    //Create server socket
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
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
    //Change the socket state to listening for client connections
    if(listen(serverSocket, 5) == -1)
        errorHandling("listen() error");

    //Accept the connection request from the first client in the waiting queue
    clientAddressSize = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,&clientAddressSize);
    if(clientSocket == -1)
        errorHandling("accept() error");

    write(clientSocket, message, sizeof(message));

    //Close the socket
    close(clientSocket);
    
    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc(' ',stderr);
    exit(1);
}