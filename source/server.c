#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include "net_helper.h"
#include "select_helper.h"

#define STDIN 0

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

static void get_cmd_ln_args(Arguments* args,int argc, char** argv);
static void server_loop(int serverSocket, int fileDescriptor);
static void handle_serversocket_activity(int fileDescriptor, int serverSocket,
    Files* files, std::map<int, ClientInfo>* clients);
static void handle_socket_activity(int fileDescriptor, int selectedSocket,
    Files* files, std::map<int, ClientInfo>* clients);
static void handle_message(int socket, std::map<int, ClientInfo>* clients,
    Message msg);
static void fatal_error(const char* errstr);

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

/**
 * parses command line arguments from argc, and argv into the args structure.
 *
 * @function   get_cmd_ln_args
 *
 * @date       2015-03-10
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  static void get_cmd_ln_args(Arguments* args,int argc, char**
 *   argv)
 *
 * @param      args pointer to an args structre.
 * @param      argc number of command line arguments.
 * @param      argv array of strings, each string is a command line argument.
 */
static void get_cmd_ln_args(Arguments* args,int argc, char** argv)
{
    char optstring[] = "p:f:";
    int ret;
    char usage[1000];

    // create usage string
    sprintf(usage,"usage: %s -p [port_number] -f [file_path]\n",argv[0]);

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
        case '?':
            printf("unrecognized switch: %s\n",optarg);
            break;
        }
    }

    // error if command line arguments aren't correct
    if (optind < argc || args->port == 0 || args->filePath == 0)
    {
        fatal_error(usage);
    }
}

/**
 * server loop, it waits for activity to happen, and once it does, it figures
 *   out where the activity is from, and handles it.
 *
 * @function   server_loop
 *
 * @date       2015-03-10
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * if activity is detected from the server socket, it will accept the
 *   connection.
 *
 * if activity is detected from stdin, execution flow will break out of the
 *   loop.
 *
 * if activity is detected from a normal socket, the server will call
 *   handle_socket_activity to handle it.
 *
 * @signature  static void server_loop(int serverSocket, int fileDescriptor)
 *
 * @param      serverSocket [description]
 * @param      fileDescriptor [description]
 */
static void server_loop(int serverSocket, int fileDescriptor)
{

    // set up the socket set & client list
    std::map<int, ClientInfo> clients;
    Files files;
    files_init(&files);

    // add the server socket to the select set
    files_add_file(&files,serverSocket);
    files_add_file(&files,STDIN);

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
            handle_serversocket_activity(fileDescriptor,serverSocket,&files,
                &clients);
            continue;
        }

        // if from stdin, exit the loop
        if(FD_ISSET(STDIN,&files.selectFds))
        {
            break;
        }

        // loop through client sockets, and handle events on them
        for(auto socketIt = files.fdSet.begin(); socketIt != files.fdSet.end();
            ++socketIt)
        {
            int selectedSocket = *socketIt;

            // if this is a client socket, and it has activity, go deal with it
            if(FD_ISSET(selectedSocket,&files.selectFds)
                && selectedSocket != serverSocket
                && selectedSocket != STDIN)
            {
                handle_socket_activity(fileDescriptor,selectedSocket,&files,
                    &clients);
            }
        }
    }
}

/**
 * handles server socket activity.
 *
 * @function   handle_serversocket_activity
 *
 * @date       2015-03-10
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  static void handle_serversocket_activity(int fileDescriptor, int
 *   serverSocket, Files* files, std::map<int, ClientInfo>* clients)
 *
 * @param      fileDescriptor file to record chat history into.
 * @param      serverSocket socket to accept the connection from.
 * @param      files set of files that select checks.
 * @param      clients maps sockets to ClientInfo structures. keeps track of all
 *   clients.
 */
static void handle_serversocket_activity(int fileDescriptor, int serverSocket,
    Files* files, std::map<int, ClientInfo>* clients)
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
    sprintf(output,"socket %d connected\n",newSocket);
    write(fileDescriptor,output,strlen(output));
    printf("%s",output);
}

/**
 * handles socket activity.
 *
 * @function   handle_socket_activity
 *
 * @date       2015-03-10
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  static void handle_socket_activity(int fileDescriptor, int
 *   selectedSocket, Files* files, std::map<int, ClientInfo>* clients)
 *
 * @param      fileDescriptor file to record chat history into.
 * @param      selectedSocket socket that is connected to the client and has
 *   data available for reading.
 * @param      files set of files that select checks.
 * @param      clients maps sockets to ClientInfo structures. keeps track of all
 *   clients.
 */
static void handle_socket_activity(int fileDescriptor, int selectedSocket,
    Files* files, std::map<int, ClientInfo>* clients)
{
    int result;
    Message msg;
    char output[1024];

    // read message from socket
    result = read_socket(selectedSocket,&msg,sizeof(msg));

    // handle socket read result
    switch(result)
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
        handle_message(selectedSocket,clients,msg);
        break;
    }
}

/**
 * handles an an application level message received from a socket connected to a
 *   client.
 *
 * @function   handle_message
 *
 * @date       2015-03-10
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note
 *
 * if the message is a chat message, it i displayed to stdout, and recorded to
 *   the specified file.
 *
 * if the message is a clientinfo message, the information is validated,
 *   possibly modified, saved into the clients map, and the ClientInfo structure
 *   is sent back to the client.
 *
 * @signature  static void handle_message(int socket, std::map<int, ClientInfo>*
 *   clients, Message msg)
 *
 * @param      socket socket that's connected to a client.
 * @param      clients maps sockets to clientinfo objects.
 * @param      msg message received from the socket.
 */
static void handle_message(int socket, std::map<int, ClientInfo>* clients,
    Message msg)
{
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

/**
 * prints out the passed error message using perror, then terminates the
 *   application.
 *
 * @function   fatal_error
 *
 * @date       2015-03-10
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
 * @param      errstr string to print out using perror.
 */
static void fatal_error(const char* errstr)
{
    perror(errstr);
    exit(errno);
}
