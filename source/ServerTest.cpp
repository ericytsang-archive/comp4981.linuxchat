#include <stdio.h>

#include "Host.h"
#include "Message.h"

#include <string.h>
#include <unistd.h>
#include <pthread.h>

using namespace Networking;

Message msg;

class TestHost : public Host
{
public:
    TestHost()
    {
    }
    virtual void onConnect(int socket)
    {
        Host::onConnect(socket);
        printf("sending message\n");
        send(socket,msg);
    }
    // virtual void onMessage(int socket, Message msg)
    // {
    // }
    // virtual void onDisconnect(int socket, int remote)
    // {
    // }
private:
};

int main(void)
{
    msg.type = 0;
    msg.data = (void*) "hey there i am eric";
    msg.len  = strlen((char*)msg.data);

    TestHost* svr = new TestHost();

    svr->startListeningRoutine(7000);
    printf("server started\n");
    getchar();

    svr->stopListeningRoutine();
    printf("server stopped\n");
    getchar();

    delete svr;
    printf("server deleted\n");
    getchar();

    return 0;
}
