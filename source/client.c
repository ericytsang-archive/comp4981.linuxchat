#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include "net_helper.h"
#include "select_helper.h"

#define STDIN 0

typedef struct
{
    char* remoteAddr;
    short port;
    char* displayName;
    char* filePath;
}
Arguments;

static void get_cmd_ln_args(Arguments* args, int argc, char** argv);
static void client_loop(int socket, int file);
static void handle_stream_activity(int socket, int file);
static void handle_socket_activity(int socket, int file);
static void handle_message(Message msg);
static void fatal_error(const char* errstr);

int main (int argc, char** argv)
{
    int socket;
    int file;
    Arguments args;
    memset(&args,0,sizeof(args));

    // get command line arguments
    get_cmd_ln_args(&args, argc, argv);

    // open file to record chat history to
    if((file = open(args.filePath, 0)) < 0)
    {
        fatal_error("failed to open the file");
    }

    // open the server socket
    socket = make_tcp_client_socket(args.remoteAddr,0,args.port,0);

    // do the server loop
    client_loop(socket,file);

    // close the server socket
    close(socket);

    // close the file
    close(file);

    return 0;
}

static void get_cmd_ln_args(Arguments* args, int argc, char** argv)
{
    char optstring[] = "p:f:d:a:";
    int ret;
    char usage[1000];

    // create usage string
    sprintf(usage,"usage: %s -p [port_number] -f [file_path] -a "
        "[remote_address] -d [display_name]\n",argv[0]);

    // parse command line arguments
    while ((ret = getopt (argc,argv,optstring)) != -1)
    {
        switch (ret)
        {
        case 'p':       // get port number
            if(!args->port)
            {
                printf("starting server on port number: %s\n",optarg);
                args->port = atoi(optarg);
            }
            else
            {
                fatal_error(usage);
            }
            break;
        case 'f':       // get file path
            if(!args->filePath)
            {
                printf("opening file for append: %s\n",optarg);
                args->filePath = optarg;
            }
            else
            {
                fatal_error(usage);
            }
            break;
        case 'd':       // get client's display name
            if(!args->displayName)
            {
                printf("display name set: %s\n",optarg);
                args->displayName = optarg;
            }
            else
            {
                fatal_error(usage);
            }
            break;
        case 'a':       // remote IP address
            if(!args->remoteAddr)
            {
                printf("remote host name: %s\n",optarg);
                args->remoteAddr = optarg;
            }
            else
            {
                fatal_error(usage);
            }
            break;
        case '?':
            printf("unrecognized switch: %s\n",optarg);
            break;
        }
    }

    // error if command line arguments aren't correct
    if (optind < argc
        || args->remoteAddr == 0
        || args->port == 0
        || args->displayName == 0
        || args->filePath == 0)
    {
        fatal_error(usage);
    }
}

static void client_loop(int socket, int file)
{
    Files files;
    files_init(&files);

    files_add_file(&files,STDIN);
    files_add_file(&files,socket);

    while(true)
    {
        // wait for an event on any socket to occur
        if(files_select(&files) == -1)
        {
            fatal_error("failed on select");
        }

        // if stream has activity, deal with it
        if(FD_ISSET(STDIN,&files.selectFds))
        {
            handle_stream_activity(socket,file);
        }

        // if socket has activity, deal with it
        if(FD_ISSET(socket,&files.selectFds))
        {
            handle_socket_activity(socket,file);
        }
    }
}

static void handle_stream_activity(int socket, int file)
{
    Message msg;    // message structure sent out socket
    char string[MSG_BUF_LEN];  // holds characters read from standard input

    // send stuff from standard input out the socket until it's empty
    do
    {
        // read from stdin
        memset(string,0,MSG_BUF_LEN);
        fgets(string,MSG_BUF_LEN,stdin);

        // set up the message structure
        msg.type = MSG_T_CHAT;
        memcpy(msg.data.message,string,MSG_BUF_LEN);

        // send the message through the socket
        send(socket,&msg,sizeof(msg),0);

        // record the message to the file
        write(file,string,strlen(string));
    }
    while(string[strlen(string)-1] != '\n');
}

static void handle_socket_activity(int socket, int file)
{
    int result;     // result of reading from the socket
    Message msg;    // message structure sent out socket
    char string[MSG_BUF_LEN];  // holds characters read from standard input

    // read from socket
    result = read_socket(socket,&msg,sizeof(msg));

    // handle read result
    switch(result)
    {
    case -1:    // socket error
    case 0:     // socket eof
        exit(0);
        break;
    default:    // socket read
        handle_message(msg);
        break;
    }
}

static void handle_message(Message msg)
{
    // parse message structure
    switch(msg.type)
    {
    case MSG_T_CHAT:
        printf("%s",msg.data.message);
        break;
    default:
        fprintf(stdout,"unknown message type received:%d\n",msg.type);
        break;
    }
}

static void fatal_error(const char* errstr)
{
    perror(errstr);
    exit(errno);
}
