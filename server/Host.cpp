/**
 * implementation of Host.h
 *
 * @sourceFile Host.cpp
 *
 * @program    N/A
 *
 * @function   Host::Host()
 * @function   Host::~Host()
 * @function   int Host::startListeningRoutine(short port)
 * @function   int Host::stopListeningRoutine()
 * @function   [some_headers_only] [class_header] [file_header]
 * @function   int Host::connect(char* remoteName, short remotePort)
 * @function   void Host::disconnect(int socket)
 * @function   void Host::onConnect(int socket)
 * @function   void Host::onMessage(int socket, Message msg)
 * @function   void Host::onDisconnect(int socket, int remote)
 * @function   int Host::startReceiveRoutine()
 * @function   int Host::stopReceiveRoutine()
 * @function   int Host::startRoutine(pthread_t* thread, void* routine, int*
 *   controlPipe, void* params)
 * @function   int Host::stopRoutine(pthread_t* thread, int* controlPipe)
 * @function   void* Host::listenRoutine(void* params)
 * @function   void* Host::receiveRoutine(void* params)
 * @function   static void fatal_error(const char* errstr)
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
#include "Host.h"
#include "net_helper.h"
#include "select_helper.h"
#include "Message.h"

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <vector>
#include <set>

/**
 * communicate to the receive thread through the receive pipe, to read a socket
 *   from the receive pipe, and add it to the set of sockets to select.
 */
#define ADD_SOCK 0

/**
 * communicate to the receive thread through the receive pipe, to read a socket
 *   from the receive pipe, and remove it from the set of sockets to select.
 */
#define RM_SOCK 1

using namespace Net;

// forward declarations
static void fatal_error(const char* errstr);

/**
 * constructs a new {Host} instance.
 *
 * @function   Host::Host
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
 *
 * @signature  Host::Host()
 */
Host::Host()
{
    svrSock = -1;
    listenThread  = 0;
    receiveThread = 0;
    startReceiveRoutine();
}

/**
 * cleans up the {Host} instance, and closes all connections that are connected
 *   to the Host instance.
 *
 * @function   Host::~Host
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
 *
 * @signature  Host::~Host()
 */
Host::~Host()
{
    stopListeningRoutine();
    stopReceiveRoutine();
}

/**
 * opens a new server socket, used to listen for new connections, and starts a
 *   new thread to listen for connection requests from that socket, and accept
 *   them.
 *
 * @function   Host::startListeningRoutine
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * returns INVALID_OPERATION when the listen thread has already started.
 *
 * returns SUCCESS when the server socket and listening thread are successful
 *   opened, and started.
 *
 * returns SOCK_OP_FAIL when the server socket fails to open. usually due to
 *   binding to an already in use port.
 *
 * @signature  int Host::startListeningRoutine(short port)
 *
 * @param      port port number to open the server socket on.
 *
 * @return     returns an integer indicating the outcome of the operation.
 */
int Host::startListeningRoutine(short port)
{
    // open the server socket
    if(listenThread == 0)
    {
        if((svrSock = make_tcp_server_socket(port)) == -1)
        {
            return SOCK_OP_FAIL;
        }
    }

    return startRoutine(&listenThread,listenRoutine,listenPipe,this);
}

/**
 * stops the listening thread, but this doesn't stop all accepted connections,
 *   it just stops the listening thread, so no new connections will be accepted.
 *
 * @function   Host::stopListeningRoutine
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * returns INVALID_OPERATION when the listening thread is already stopped, and
 *   cannot be stopped again.
 *
 * returns SUCCESS when the listening thread was successfully stopped.
 *
 * @signature  int Host::stopListeningRoutine()
 *
 * @return     integer indicating the outcome of the operation.
 */
int Host::stopListeningRoutine()
{
    return stopRoutine(&listenThread,listenPipe);
}

/**
 * sends a message to the remote {Host} instance, using the protocol that {Host}
 *   instances use.
 *
 * @function   [class_header] [method_header]
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
 *
 * @signature  [some_headers_only] [class_header] [file_header]
 *
 * @param      socket socket to send the data to.
 * @param      msg message to send to the remote host.
 */
void Host::send(int socket, Message msg)
{
    write(socket,&msg.type,sizeof(msg.type));
    write(socket,&msg.len,sizeof(msg.len));
    write(socket,msg.data,msg.len);
}

/**
 * tries to connect to a remote listening {Host} instance. this is a blocking
 *   function that runs on the main thread. when a connection is established
 *   with the remote host, onConnect will be invoked with the new socket created
 *   from the connection.
 *
 * @function   Host::connect
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * returns SUCCESS when we successfully connect to the remote {Host} instance.
 *
 * returns SOCK_OP_FAIL when we fail to make a connection with the remote host.
 *
 * @signature  int Host::connect(char* remoteName, short remotePort)
 *
 * @param      remoteName name of the remote host. can be dotted IP format, in
 *   string form, or their actual name.
 * @param      remotePort remote port number to connect to.
 *
 * @return     an integer indicating the outcome of the operation.
 */
int Host::connect(char* remoteName, short remotePort)
{
    // connect to remote host
    int socket = make_tcp_client_socket(remoteName,0,remotePort,0);

    if(socket != -1)
    {
        // communicate to receive thread that a new socket is connected
        char commandType = ADD_SOCK;
        write(receivePipe[1],&commandType,sizeof(commandType));
        write(receivePipe[1],&socket,sizeof(socket));
    }

    return (socket != -1) ? SUCCESS : SOCK_OP_FAIL;
}

/**
 * communicates to the receive thread to remove an existing socket. when the
 *   socket is disconnected, the onDisconnect callback will be invoked.
 *
 * @function   Host::disconnect
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
 *
 * @signature  void Host::disconnect(int socket)
 *
 * @param      socket socket to shutdown and close.
 */
void Host::disconnect(int socket)
{
    // communicate to receive thread to remove an existing socket
    char commandType = RM_SOCK;
    write(receivePipe[1],&commandType,sizeof(commandType));
    write(receivePipe[1],&socket,sizeof(socket));
}

/**
 * callback invoked when a new connection is established from the remote host,
 *   or a new connection request is made on the listening socket.
 *
 * this implementation just prints a message that a socket has connected.
 *
 * @function   Host::onConnect
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
 *
 * @signature  void Host::onConnect(int socket)
 *
 * @param      socket new socket that was created to communicate with the
 *   connection.
 */
void Host::onConnect(int socket)
{
    printf("server: socket %d connected\n",socket);
}

/**
 * callback invoked when a new message has been received on a connection. the
 *   received message must match the protocol that Host instances use, or this
 *   wont work.
 *
 * this implementation just prints the message that was received from the remote
 *   host, and which socket it is from.
 *
 * @function   Host::onMessage
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
 *
 * @signature  void Host::onMessage(int socket, Message msg)
 *
 * @param      socket socket that the message was received from.
 * @param      msg message structure received through the socket.
 */
void Host::onMessage(int socket, Message msg)
{
    printf("server: socket %d: msg.type: %d, msg.data: ",socket,msg.type);
    for(int i = 0; i < msg.len; ++i)
    {
        printf("%c",((char*)msg.data)[i]);
    }
    printf("\n");
}

/**
 * callback invoked when a connection is terminated.
 *
 * this implementation just prints message that the socket was disconnected by
 *   the remote, or local host.
 *
 * @function   Host::onDisconnect
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
 *
 * @signature  void Host::onDisconnect(int socket, int remote)
 *
 * @param      socket socket that was disconnected
 * @param      remote true if the connection was terminated by the remote host,
 *   false otherwise.
 */
void Host::onDisconnect(int socket, int remote)
{
    printf("server: socket %d disconnected by %s host\n",
        socket,remote?"remote":"local");
}

/**
 * starts the {Host} instance's receive thread if it isn't already started.
 *
 * @function   Host::startReceiveRoutine
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * returns SUCCESS when the receive thread is successful started.
 *
 * returns INVALID_OPERATION when the receive thread could not be started
 *   because it is already started.
 *
 * @signature  int Host::startReceiveRoutine()
 *
 * @return     an integer value indicating the outcome of the operation.
 */
int Host::startReceiveRoutine()
{
    return startRoutine(&receiveThread,receiveRoutine,receivePipe,this);
}
/**
 * stops the receive routine  if it's not already stopped.
 *
 * @function   Host::stopReceiveRoutine
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * returns INVALID_OPERATION when the listening thread is already stopped, and
 *   cannot be stopped again.
 *
 * returns SUCCESS when the listening thread was successfully stopped.
 *
 * @signature  int Host::stopReceiveRoutine()
 *
 * @return     an integer indicating the outcome of the operation.
 */
int Host::stopReceiveRoutine()
{
    return stopRoutine(&receiveThread,receivePipe);
}

/**
 * starts the passed routine on a new thread. the thread id will be assigned to
 *   the passed thread id pointer.
 *
 * @function   Host::startRoutine
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   EricTsang
 *
 * @programmer EricTsang
 *
 * @note
 *
 * returns SUCCESS when the receive thread is successful started.
 *
 * returns INVALID_OPERATION when the receive thread could not be started
 *   because it is already started.
 *
 * @signature  int Host::startRoutine(pthread_t* thread, void* routine, int*
 *   controlPipe, void* params)
 *
 * @param      thread pointer to the thread id variable.
 * @param      routine function to execute on the thread.
 * @param      controlPipe pointer to an int[2] that holds the file descriptors
 *   of a unnamed FIFO unnamed pipe.
 * @param      params parameters to pass to the new thread.
 *
 * @return     an integer indicating the outcome of the operation.
 */
int Host::startRoutine(pthread_t* thread, void*(*routine)(void*),
    int* controlPipe, void* params)
{
    // return immediately if the routine is already running
    if(*thread != 0)
    {
        return INVALID_OPERATION;
    }

    // create the control pipe
    if(pipe(controlPipe) == SYS_ERROR)
    {
        fatal_error("failed to create the control pipe");
    }

    // start the thread
    pthread_create(thread,0,routine,params);
    return SUCCESS;
}

/**
 * stops a thread from executing a routine by communicating to it through an IPC
 *   pipe.
 *
 * @function   Host::stopRoutine
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   EricTsang
 *
 * @programmer EricTsang
 *
 * @note
 *
 * returns INVALID_OPERATION when the listening thread is already stopped, and
 *   cannot be stopped again.
 *
 * returns SUCCESS when the listening thread was successfully stopped.
 *
 * @signature  int Host::stopRoutine(pthread_t* thread, int* controlPipe)
 *
 * @param      thread pointer to a thread id variable.
 * @param      controlPipe pointer to an integer array of size 2, used to hold
 *   the read and write file descriptors of a pipe.
 *
 * @return     integer values indicating the outcome of the operation.
 */
int Host::stopRoutine(pthread_t* thread, int* controlPipe)
{
    // return immediately if the routine is already stopped
    if(*thread == 0)
    {
        return INVALID_OPERATION;
    }

    // close the control pipe which terminates the thread
    close(controlPipe[1]);
    pthread_join(*thread,0);

    // set thread to 0, so we know it's terminated
    *thread = 0;
    return SUCCESS;
}

/**
 * function run on a thread. it polls the server socket, and a control pipe,
 *   accepting connections, and waiting for further commands from the control
 *   pipe.
 *
 * @function   Host::listenRoutine
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
 *
 * @signature  void* Host::listenRoutine(void* params)
 *
 * @param      params pointer to the {Host} instance the thread belongs to.
 */
void* Host::listenRoutine(void* params)
{
    printf("listenroutine started...\n");
    fflush(stdout);
    // parse thread parameters
    Host* dis = (Host*) params;

    int terminateThread = 0;

    // set up the socket set & client list
    Files files;
    files_init(&files);

    // add the server socket and control pipe to the select set
    files_add_file(&files,dis->svrSock);
    files_add_file(&files,dis->listenPipe[0]);

    // accept any connection requests, and create a session for each
    while(!terminateThread && dis->svrSock != -1)
    {
        // wait for an event on any socket to occur
        if(files_select(&files) == -1)
        {
            fatal_error("failed on select");
        }

        // loop through sockets, and handle them
        for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
            ++socketIt)
        {
            int curSock = *socketIt;

            // if this socket doesn't have any activity, move on to next socket
            if(!FD_ISSET(curSock,&files.selectFds))
            {
                continue;
            }

            // handle socket activity depending on which socket it is
            if(curSock == dis->svrSock)
            {
                /*
                 * this is the server socket, try to accept a connection.
                 *
                 * if the operation fails, end the server thread, because when
                 *   accept fails, it means that the server socket is closed.
                 *
                 * if accept succeeds, add it to the select set, and continue
                 *   looping...
                 */

                // accept the connection
                int newSock;
                if((newSock = accept(dis->svrSock,0,0)) == -1)
                {
                    // accept failed; server socket closed, terminate thread
                    terminateThread = 1;
                }
                else
                {
                    // accept success; add the socket to the receive thread.
                    char commandType = ADD_SOCK;
                    write(dis->receivePipe[1],&commandType,sizeof(commandType));
                    write(dis->receivePipe[1],&newSock,sizeof(newSock));
                }
            }

            if(curSock == dis->listenPipe[0])
            {
                /*
                 * this is the control pipe. whenever anything happens on the
                 *   control pipe, it means it's time for the server to
                 *   shutdown; break out of the server loop.
                 */

                 terminateThread = 1;
            }
        }
    }

    // close all file descriptors before terminating
    for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
        ++socketIt)
    {
        close(*socketIt);
    }

    printf("listenroutine stopped...\n");
    fflush(stdout);

    return 0;
}

/**
 * function run on a thread. it polls a set of connected sockets, and a control
 *   pipe.
 *
 * @function   Host::receiveRoutine
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
 *
 * @signature  void* Host::receiveRoutine(void* params)
 *
 * @param      params pointer to the {Host} instance the thread belongs to.
 */
void* Host::receiveRoutine(void* params)
{
    printf("receiveroutine started...\n");
    fflush(stdout);

    // parse thread parameters
    Host* dis = (Host*) params;

    // used to break the while loop
    int terminateThread = 0;

    // set of sockets that have been shutdown from local host
    std::set<int> shutdownSocks;

    // set up the socket set & client list
    Files files;
    files_init(&files);

    // add the server socket and control pipe to the select set
    files_add_file(&files,dis->receivePipe[0]);

    // accept any connection requests, and create a session for each
    while(!terminateThread)
    {
        // wait for an event on any socket to occur
        if(files_select(&files) == -1)
        {
            fatal_error("failed on select");
        }

        // loop through sockets, and handle them
        for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
            ++socketIt)
        {
            int curSock = *socketIt;

            // if this socket doesn't have any activity, move on to next socket
            if(!FD_ISSET(curSock,&files.selectFds))
            {
                continue;
            }

            // handle socket activity depending on which socket it is
            if(curSock == dis->receivePipe[0])
            {
                /*
                 * this is the control pipe. try to read from the control pipe.
                 *
                 * if the control pipe is closed, the client is being deleted;
                 *   this client thread should terminate.
                 *
                 * if the control pipe is read, that means that a new socket is
                 *   now connected, and needs to be added to the selection set.
                 */

                char cmdType;
                if(read_file(dis->receivePipe[0],&cmdType,sizeof(cmdType)) == 0)
                {
                    // pipe closed; the client is being deleted, thread should
                    // terminate
                    terminateThread = 1;
                }
                else
                {
                    // pipe read; read a socket from the pipe, and depending on
                    // the cmdType, do something with it
                    int socket;
                    read_file(dis->receivePipe[0],&socket,sizeof(socket));
                    switch(cmdType)
                    {
                    case ADD_SOCK:
                        files_add_file(&files,socket);
                        dis->onConnect(socket);
                        break;
                    case RM_SOCK:
                        if(files.fdSet.find(socket) != files.fdSet.end())
                        {
                            shutdown(socket,SHUT_RDWR);
                            shutdownSocks.insert(socket);
                        }
                        break;
                    }
                }
            }
            else
            {
                /*
                 * this is the client socket; try to read from the socket
                 *
                 * if read fails, it means that the socket is closed, remove it
                 *   from the select set, call a callback, and continue looping.
                 *
                 * if read succeeds, read until socket is empty, and call
                 *   callback, then continue looping.
                 */

                // read from socket
                Message msg;
                if(read_file(curSock,&msg.type,sizeof(msg.type)) == 0)
                {
                    // socket closed; remove from select set, and call callback
                    files_rm_file(&files,curSock);
                    int remote = (shutdownSocks.erase(curSock) == 0
                        && close(curSock) == 0);
                    dis->onDisconnect(curSock,remote);
                    close(curSock);
                }
                else
                {
                    // socket read; read more from socket, and call callback
                    read_file(curSock,&msg.len,sizeof(msg.len));

                    char* buffer = (char*) malloc(msg.len);
                    read_file(curSock,buffer,msg.len);
                    msg.data = buffer;

                    dis->onMessage(curSock,msg);

                    //free(buffer);
                }
                break;
            }
        }
    }

    // close all sockets before terminating
    for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
        ++socketIt)
    {
        int curSock = *socketIt;
        close(curSock);
        if(curSock != dis->receivePipe[0])
        {
            dis->onDisconnect(curSock,0);
        }
    }

    printf("receiveroutine stopped...\n");
    fflush(stdout);

    return 0;
}

/**
 * prints the the passed string, and then exits the process.
 *
 * @function   fatal_error
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
 *
 * @signature  static void fatal_error(const char* errstr)
 *
 * @param      errstr string to print using perror before exiting the process.
 */
static void fatal_error(const char* errstr)
{
    perror(errstr);
    exit(errno);
}
