/**
 * exposes interface of net_helper.cpp
 *
 * @sourceFile net_helper.h
 *
 * @program    N/A
 *
 * @function   int make_tcp_server_socket(short port)
 * @function   int make_tcp_client_socket(char* remoteName, long remoteAddr,
 *   short remotePort, short localPort)
 * @function   struct sockaddr make_sockaddr(char* hostName, long hostAddr,
 *   short hostPort)
 * @function   int read_file(int socket, void* bufferPointer, int bytesToRead)
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 */
#ifndef _NET_HELPER_H_
#define _NET_HELPER_H_

int make_tcp_server_socket(short port);
int make_tcp_client_socket(char* remoteName, long remoteAddr, short remotePort, short localPort);
struct sockaddr make_sockaddr(char* hostName, long hostAddr, short hostPort);
int read_file(int socket, void* bufferPointer, int bytesToRead);

#endif
