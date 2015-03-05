#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

int make_server_socket(short port);
int make_client_socket(long remoteAddr, short remotePort, short localPort);
struct sockaddr make_sockaddr(char* remoteName, long remoteAddr, short remotePort);
