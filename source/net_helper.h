#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MSG_BUF_LEN 80

#define MSG_T_CHAT 0

typedef struct
{
    char name[80];
} ClientInfo;

typedef union
{
    char message[MSG_BUF_LEN];
} Payload;

typedef struct
{
    char type;
    Payload data;
} Message;

int make_tcp_server_socket(short port);
int make_tcp_client_socket(char* remoteName, long remoteAddr, short remotePort, short localPort);
struct sockaddr make_sockaddr(char* hostName, long hostAddr, short hostPort);
int read_socket(int socket, void* bufferPointer, int bytesToRead);
