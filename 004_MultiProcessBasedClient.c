#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

# define BUFFER_SIZE 1024

void errorHandling(char *message);
void readRoutine(int socket, char *message);
void writeRoutine(int socket, char *message);

int main(int argc, char **argv)
{
    int clientSocket;
    struct sockaddr_in serverAddress;

    pid_t processId;
    char message[BUFFER_SIZE];

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
    
    //Create a child process to....
    processId = fork();
    if (processId == -1)
    {
        close(clientSocket);
        errorHandling("fork() error");
    }

    if(processId == 0)
    {
        //Child process: write only
        writeRoutine(clientSocket, message);
    }
    else
    {
        //Parent process: read only
        readRoutine(clientSocket, message);
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

void readRoutine(int socket, char *message)
{
    int strLength;
    while ((strLength = read(socket, message, BUFFER_SIZE)) > 0)
    {
        message[strLength] = '\0';
        printf("Message from server : %s\n", message);
    }
    
    printf("Server closed output stream\n");
}

void writeRoutine(int socket, char *message)
{
    while(1)
    {
        fputs("Input message: ", stdout);
        fgets(message, BUFFER_SIZE, stdin);

        if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
        {            
            return;
        }
        write(socket, message, strlen(message));
    }
    shutdown(socket, SHUT_WR);
    close(socket);
}