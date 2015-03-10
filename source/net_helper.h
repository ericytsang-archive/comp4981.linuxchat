#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct
{
    char name[80];
} ClientInfo;

typedef struct
{
    int type;
    char message[80];
} Message;

int make_tcp_server_socket(short port);
int make_tcp_client_socket(char* remoteName, long remoteAddr, short remotePort, short localPort);
struct sockaddr make_sockaddr(char* hostName, long hostAddr, short hostPort);
