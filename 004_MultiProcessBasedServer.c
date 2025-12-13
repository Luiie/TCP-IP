#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

# define BUFFER_SIZE 1024

void errorHandling(char *message);
void readChildProcess(int signal);

int main(int argc,char *argv[])
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;

    pid_t processId;
    struct sigaction action;

    socklen_t clientAddressSize;
    int strLength, state;
    char message[BUFFER_SIZE];

    if(argc != 2)
    {
        printf("Usage : %s <port> ",argv[0]);
        exit(1);
    }

    //Register a signal handler to prevent zombie processes
    action.sa_handler = readChildProcess;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    state = sigaction(SIGCHLD, &action, 0);

    //Create a TCP server socket
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1)
        errorHandling("socket() error");
    //Initialize server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(atoi(argv[1]));

    //Bind the socket to the given IP address and port
    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1)
        errorHandling("bind() error");
    //Put the socket into listening state to accept client connections
    if(listen(serverSocket, 5) == -1)
        errorHandling("listen() error");

    //Continuously accept client connection requests
    while(1)
    {
        clientAddressSize = sizeof(clientAddress);
        //Accept a new client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if(clientSocket == -1)
            errorHandling("accept() error");
        else
            printf("Connected: client\n");
        
        //Create a child process to handle the connected client
        processId = fork();
        if (processId == -1)
        {
            close(clientSocket);
            errorHandling("fork() error");
        }

        if(processId == 0)
        {
            //Child process: handle communication with the client
            close(serverSocket);

            while((strLength=read(clientSocket, message, BUFFER_SIZE)) != 0)
            {
                write(clientSocket, message, strLength);
            }
            printf("Message from client : %s", message);

            close(clientSocket);
        }
        else
        {
            //Parent process: close connected socket
            close(clientSocket);
        }
    }

    //Close connected socket
    close(serverSocket);
    
    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc(' ',stderr);
    exit(1);
}

void readChildProcess(int signal)
{
    pid_t processId;
    int status;
    processId = waitpid(-1, &status, WNOHANG);
}