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
    struct sockaddr_in serverAddress;
    char message[BUFFER_SIZE];
    int strLength = 0;
    int recvLength = 0, recvCounter;

    if(argc != 3)
    {
        printf("usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // Create client socket
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1)
        errorHandling("socket() error");

    //Initialize server socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));

    //Connect to the server
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
        errorHandling("connect() error!!");
    else
        puts("Connected!");
    
    //Receive data byte-by-byte with boundary check
    while(1)
    {
        fputs("Input message: ", stdout);
        fgets(message, BUFFER_SIZE, stdin);

        if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break;
        
        strLength = write(clientSocket, message, strlen(message));
        //Read as much data as has been written to the message.
        while(recvLength < strLength)
        {            
            recvCounter = read(clientSocket, message, BUFFER_SIZE-1);
            if(recvCounter == -1)
                errorHandling("read() error!");
            recvLength += recvCounter;
        }
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