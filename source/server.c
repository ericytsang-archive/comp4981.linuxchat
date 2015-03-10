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

    files_add_file(&files, serverSocket);

    while(true)
    {
        // wait for an event on any socket to occur
        if(files_select(&files) == -1)
        {
            fatal_error("failed on select");
        }

        if(FD_ISSET(serverSocket, &files.selectFds))
        {
            // accept the connection, add it to socket set, & client list
            int newSocket = accept(serverSocket, 0, 0);
            files_add_file(&files, newSocket);
            ClientInfo newClient;
            clients[newSocket] = newClient;
            continue;
        }

        for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
            ++socketIt)
        {
            int selectedSocket = *socketIt;
            ClientInfo* client = &clients[selectedSocket];
            char output[1024];
            int bytesToRead;
            int bytesRead;
            char* bufferPointer;
            Message msg;

            if(selectedSocket == serverSocket
                || !FD_ISSET(selectedSocket, &files.selectFds))
            {
                continue;
            }

            // read message from socket
            bufferPointer = (char*) &msg;
            bytesToRead = sizeof(Message);
            while ((bytesRead = read(selectedSocket, bufferPointer, bytesToRead)) > 0)
            {
                bufferPointer += bytesRead;
                bytesToRead -= bytesRead;
            }

            // handle socket read result
            switch(bytesRead)
            {
            case -1:
            case 0:
                // remove the socket from socket set and socket list
                files_rm_file(&files, selectedSocket);
                clients.erase(selectedSocket);

                // write update to file
                sprintf(output, "socket %d disconnected\n", selectedSocket);
                write(fileDescriptor, output, strlen(output));
                printf("%s", output);

                // close the socket
                close(selectedSocket);
                break;
            default:
                send(selectedSocket, "output", strlen("output"), 0);
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
    }
}

static void get_cmd_ln_args(Arguments* args, int argc, char** argv)
{
    char optstring[] = "p:f:";
    int ret;

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
                fatal_error("failed to parse command line arguments");
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
                fatal_error("failed to parse command line arguments");
            }
            break;
        case '?':
            printf("unrecognized switch: %s\n", optarg);
            break;
        }
    }

    if (optind < argc)
    {
        fatal_error("failed to parse command line arguments");
    }
}

static void fatal_error(const char* errstr)
{
    perror(errstr);
    exit(errno);
}
