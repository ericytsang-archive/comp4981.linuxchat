#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

// globals for getopt
extern char *optarg = 0;
extern int optind   = 0;
extern int opterr   = 0;

typedef struct
{
    short port;
    char* filePath;
}
Arguments;

static void fatal_error(const char* errstr);
static void get_cmd_ln_args(Arguments* args, int argc, char** argv);

int main (int argc, char** argv)
{
    int serverSocket;
    Arguments args;

    get_cmd_ln_args(&args, argc, argv);

    // open file to record chat history to
    open(/*file*/);

    // open the server socket
    serverSocket = make_tcp_server_socket(args->port);

    // do the server loop
    server_loop(serverSocket);

    // close the server socket
    close(serverSocket);

    // close the file
    close(/*file*/);

    return 0;
}

static void server_loop(int serverSocket, int fileDescriptor)
{

    // set up the socket set

    while(?)
    {
        // wait for an event on any socket to occur

        if(FD_ISSET(serverSocket))
        {
            // accept the connection
            // add connection to socket set
        }

        for(/*loop through the rest of the sockets*/)
        {
            if(!FD_ISSET(/*currentSocket*/))
            {
                continue;
            }

            // read from the socket
            switch(/*read result*/)
            {
            case /*error*/:
            case /*eof*/:
                // remove the socket from socket set
                // remove the socket from socket list
                // write update to file
                // close the socket
                break;
            case /*sucess*/:
                if(/*control message*/)
                {
                    // add connection to socket list
                    // write update to file
                }
                else
                {
                    send_to_all_except(/*read data*/, /*sending socket*/);
                    // write update to file
                }
                break;
            }
        }
    }
}

static void get_cmd_ln_args(Arguments* args, int argc, char** argv)
{
    char optstring[] = "p:f";
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
            printf("%s\n", "unrecognized switch: ", optarg);
            break;
        }
    }

    fatal_error("failed to parse command line arguments");
}

static void fatal_error(const char* errstr)
{
    perror(errstr);
    exit(errno);
}
