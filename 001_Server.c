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
    int serverSock;
    int clintSock;
    struct sockaddr_in serverAddress;
    struct sockaddr_in cilntAddress;
    int cilntAddressSize;
    char message[]="Hello HANY!";

    if(argc =! 2)
    {
        printf("Usage : %s <port> ",argv[0]);
       exit(1);
    }

    //Create server socket
    serverSock = socket(PF_INET, SOCK_STREAM, 0);
    if(serverSock == -1)
        errorHandling("socket() error");
    
    //Initialize server socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(atoi(argv[1]));

    //Assign an address to socket
    if(bind(serverSock, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1)
        errorHandling("bind() error");
    if(listen(serverSock, 5) == -1)      //대기상태
        errorHandling("listen() error");
    cilntAddressSize = sizeof(cilntAddress);
    clintSock = accept(serverSock, (struct sockaddr*)&cilntAddress,&cilntAddressSize);        //요청 수락

    if(clintSock == -1)
        errorHandling("accept() error");
    write(clintSock, message, sizeof(message));
    close(clintSock);       //연결 종료
    return 0;
}

void errorHandling(char *message)
{
    fputs(message, stderr);
    fputc(' ',stderr);
    exit(1);
}