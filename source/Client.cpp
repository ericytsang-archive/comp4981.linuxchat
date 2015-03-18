#include "Client.h"
#include "Message.h"
#include "protocol.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

Client::Client()
{
    name = "name";
}

Client::~Client()
{
}

void Client::onConnect(int socket)
{
    Host::onConnect(socket);
    // send the server your user name
    Net::Message msg;
    msg.type = CHECK_USR_NAME;
    msg.data = name;
    msg.len = strlen(name);
    send(socket,msg);
    svrSock = socket;
}

void Client::onMessage(int socket, Net::Message msg)
{
    Host::onMessage(socket,msg);
    switch(msg.type)
    {
    case ADD_CLIENT:
        onAddClient((char*)msg.data);
        break;
    case RM_CLIENT:
        onRmClient((char*)msg.data);
        break;
    case SHOW_MSG:
        onShowMessage((char*)msg.data);
        break;
    case SET_USR_NAME:
        onSetName((char*)msg.data);
        break;
    }
}

void Client::onDisconnect(int socket, int remote)
{
    Host::onDisconnect(socket,remote);
}

void Client::sendChatMessage(char* chatMsg)
{
    Net::Message msg;
    msg.type = SHOW_MSG;
    msg.data = chatMsg;
    msg.len = strlen(chatMsg);
    send(svrSock,msg);
}

void Client::onAddClient(char* clientName)
{
    printf("%s has connected.\n",clientName);
}

void Client::onRmClient(char* clientName)
{
    printf("%s has disconnected.\n",clientName);
}

void Client::onShowMessage(char* message)
{
    printf("%s\n",message);
}

void Client::onSetName(char* newName)
{
    printf("your name is %s.\n",newName);
}

int main(void)
{
    Client* clnt = new Client();
    clnt->connect("localhost",7000);

    Net::Message chatMsg;
    chatMsg.type = SHOW_MSG;
    chatMsg.len  = 1024;
    chatMsg.data = malloc(chatMsg.len);
    while(fgets((char*)chatMsg.data,chatMsg.len,stdin))
    {
        if(strcmp((char*)chatMsg.data,"exit\n") == 0)
        {
            break;
        }

        clnt->sendChatMessage((char*)chatMsg.data);
    }

    delete clnt;
    printf("client disconnected\n");

    return 0;
}
