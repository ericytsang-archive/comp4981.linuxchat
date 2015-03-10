#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include "net_helper.h"
#include "select_helper.h"

// globals for getopt
extern char *optarg;
extern int optind;
extern int opterr;

typedef struct
{
    short port;
    char* filePath;
}
Arguments;

static void fatal_error(const char* errstr);
static void get_cmd_ln_args(Arguments* args, int argc, char** argv);
static void server_loop(int serverSocket, int fileDescriptor);
static void handle_serversocket_activity(int serverSocket, Files* files,
    std::map<int, ClientInfo>* clients);
static void handle_socket_activity(int selectedSocket, int fileDescriptor,
    Files* files, std::map<int, ClientInfo>* clients);

int main (int argc, char** argv)
{
    int serverSocket;
    int fileDescriptor;
    Arguments args;
    args.port = 0;
    args.filePath = 0;

    // get command line arguments
    get_cmd_ln_args(&args, argc, argv);

    // open file to record chat history to
    if((fileDescriptor = open(args.filePath, 0)) < 0)
    {
        fatal_error("failed to open the file");
    }

    // open the server socket
    serverSocket = make_tcp_server_socket(args.port);

    // do the server loop
    server_loop(serverSocket, fileDescriptor);

    // close the server socket
    close(serverSocket);

    // close the file
    close(fileDescriptor);

    return 0;
}

static void server_loop(int serverSocket, int fileDescriptor)
{

    // set up the socket set & client list
    std::map<int, ClientInfo> clients;
    Files files;
    files_init(&files);

    // add the server socket to the select set
    files_add_file(&files, serverSocket);

    while(true)
    {
        // wait for an event on any socket to occur
        if(files_select(&files) == -1)
        {
            fatal_error("failed on select");
        }

        // if server socket has activity, accept the connection, add it to
        // socket set, & client list
        if(FD_ISSET(serverSocket,&files.selectFds))
        {
            handle_serversocket_activity(serverSocket,&files,&clients);
            continue;
        }

        // loop through client sockets, and handle events on them
        for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
            ++socketIt)
        {
            int selectedSocket = *socketIt;

            // if there is no activity on the socket, or socket is server
            // socket, go do the next socket
            if(selectedSocket == serverSocket
                || !FD_ISSET(selectedSocket,&files.selectFds))
            {
                continue;
            }

            handle_socket_activity(selectedSocket,fileDescriptor,&files,
                &clients);
        }
    }
}

static void handle_serversocket_activity(int serverSocket, Files* files,
    std::map<int, ClientInfo>* clients)
{
    char output[1024];

    // accept the new connection
    int newSocket = accept(serverSocket,0,0);
    files_add_file(files,newSocket);

    // create new ClientInto structure to keep track of state information
    // associated with new connection
    ClientInfo newClient;
    (*clients)[newSocket] = newClient;

    // write to file and stdout
    sprintf(output,"socket %d connected\n",selectedSocket);
    write(fileDescriptor,output,strlen(output));
    printf("%s",output);
}

static void handle_socket_activity(int selectedSocket, int fileDescriptor,
    Files* files, std::map<int, ClientInfo>* clients)
{
    int bytesToRead;
    int bytesRead;
    char* bufferPointer;
    Message msg;
    char output[1024];

    // read message from socket
    bufferPointer = (char*) &msg;
    bytesToRead = sizeof(Message);
    while ((bytesRead = read(selectedSocket,bufferPointer,bytesToRead)) > 0)
    {
        bufferPointer += bytesRead;
        bytesToRead -= bytesRead;
    }

    // handle socket read result
    switch(bytesRead)
    {
    case -1:
    case 0:
        // remove the socket from the select set and socket list, and close it
        files_rm_file(files,selectedSocket);
        close(selectedSocket);
        clients->erase(selectedSocket);

        // write to file and stdout
        sprintf(output,"socket %d disconnected\n",selectedSocket);
        write(fileDescriptor,output,strlen(output));
        printf("%s",output);
        break;
    default:
        send(selectedSocket,"output", strlen("output"), 0);
        // if(/*control message*/)
        // {
        //     // add connection to socket list
        //     // write update to file
        // }
        // else
        // {
        //     // send_to_all_except(/*read data*/, /*sending socket*/);
        //     // write update to file
        // }
        break;
    }
}

static void get_cmd_ln_args(Arguments* args, int argc, char** argv)
{
    char optstring[] = "p:f:";
    int ret;
    char usage[1000];

    // create usage string
    sprintf(usage,"usage: %s -p [port_number] -f [file_path]\n",argv[0]);

    // parse command line arguments
    while ((ret = getopt (argc, argv, optstring)) != -1)
    {
        switch (ret)
        {
        case 'p':
            if(!args->port)
            {
                printf("starting server on port number: %s\n", optarg);
                args->port = atoi(optarg);
            }
            else
            {
                fatal_error(usage);
            }
            break;
        case 'f':
            if(!args->filePath)
            {
                printf("opening file for append: %s\n", optarg);
                args->filePath = optarg;
            }
            else
            {
                fatal_error(usage);
            }
            break;
        case '?':
            printf("unrecognized switch: %s\n", optarg);
            break;
        }
    }

    // error if command line arguments aren't correct
    if (optind < argc || args->port == 0 || args->filePath == 0)
    {
        fatal_error(usage);
    }
}

static void fatal_error(const char* errstr)
{
    perror(errstr);
    exit(errno);
}
