#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void errorHandling(char *message);

int main(int argc, char **argv)
{
    int aSocket;
    struct sockaddr_in serverAddress;
    char message[30];
    int strLength = 0;
    int index = 0, readLength = 0;

    if(argc != 3)
    {
        printf("usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // Create client socket
    aSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(aSocket == -1)
        errorHandling("socket() error");

    //Initialize server socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));

    //Connect to the server
    if(connect(aSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
        errorHandling("connect() error!!");

    //Receive data byte-by-byte with boundary check
    while(readLength = read(aSocket, &message[index++], 1))
    {
        if(strLength == -1)
            errorHandling("read() error!");
        
        strLength += readLength;
    }
    message[strLength] = '\0';

    printf("Message from server : %s \n", message);
    printf("Function read call counter : %d \n", strLength);

    //Close the socket
    close(aSocket);

    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}