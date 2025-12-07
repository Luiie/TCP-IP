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
    FILE *filePointer;
    struct sockaddr_in serverAddress;
    char message[BUFFER_SIZE];
    int readCount;

    if(argc != 3)
    {
        printf("usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // Create client socket & file
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1)
        errorHandling("socket() error");
    filePointer = fopen("receive.dat", "wb");
    if(filePointer == NULL)
    {
        perror("fopen");
        exit(1);
    }

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
    
    //Receive data
    while((readCount = read(clientSocket, message, BUFFER_SIZE)) != 0)
    {
        fwrite((void*)message, 1, readCount, filePointer);
    }

    puts("Receive file data");
    write(clientSocket, "Fare well", strlen("Fare well"));

    //Close the socket & file
    close(clientSocket);
    fclose(filePointer);

    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}