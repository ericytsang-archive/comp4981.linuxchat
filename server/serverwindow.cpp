/**
 * [void description]
 *
 * @sourceFile serverwindow.cpp
 *
 * @program    server
 *
 * @function   ServerWindow::ServerWindow(QWidget *parent)
 * @function   ServerWindow::~ServerWindow()
 * @function   void ServerWindow::addUserListEntry(int key, QString usrName)
 * @function   void ServerWindow::rmUserListEntry(int key)
 * @function   void ServerWindow::appendText(char* str)
 * @function   void ServerWindow::onConnect(int socket)
 * @function   void ServerWindow::onMessage(int socket, Net::Message msg)
 * @function   void ServerWindow::onDisconnect(int socket, int remote)
 * @function   void ServerWindow::on_actionConnect_triggered()
 * @function   void ServerWindow::on_actionDisconnect_triggered()
 * @function   void ServerWindow::on_actionSettings_triggered()
 * @function   void ServerWindow::onShowMessage(int socket, char* cstr)
 * @function   void ServerWindow::onCheckUserName(int socket, char* cname)
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 */
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "dialog.h"
#include "protocol.h"
#include "Message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <qmessagebox.h>
#include <qscrollbar.h>
#include <unistd.h>

////////////////////////////////
// constructors & destructors //
////////////////////////////////

/**
 * [ServerWindow::ServerWindow description]
 *
 * @function   ServerWindow::ServerWindow
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  ServerWindow::ServerWindow(QWidget *parent)
 *
 * @param      parent [description]
 */
ServerWindow::ServerWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

     // initial settings
    ui->actionDisconnect->setEnabled(false);

    // initialize instance variables
    port = 7000;
    file = -1;
    filePath = "./test";
}

/**
 * [ServerWindow::ServerWindow description]
 *
 * @function   ServerWindow::~ServerWindow
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  ServerWindow::~ServerWindow()
 */
ServerWindow::~ServerWindow()
{
    delete ui;
}

//////////////////////
// public interface //
//////////////////////

/**
 * [ServerWindow::addUserListEntry description]
 *
 * @function   ServerWindow::addUserListEntry
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::addUserListEntry(int key, QString usrName)
 *
 * @param      key [description]
 * @param      usrName [description]
 */
void ServerWindow::addUserListEntry(int key, QString usrName)
{
    QListWidgetItem* li = new QListWidgetItem(usrName);
    lis.insert(key,li);
    ui->listWidget->addItem(li);
}

/**
 * [ServerWindow::rmUserListEntry description]
 *
 * @function   ServerWindow::rmUserListEntry
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::rmUserListEntry(int key)
 *
 * @param      key [description]
 */
void ServerWindow::rmUserListEntry(int key)
{
    ui->listWidget->removeItemWidget(lis[key]);
    delete lis[key];
    lis.remove(key);
}

/**
 * [ServerWindow::appendText description]
 *
 * @function   ServerWindow::appendText
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::appendText(char* str)
 *
 * @param      str [description]
 */
void ServerWindow::appendText(char* str)
{
    // append the passed text onto the screen
    ui->textBrowser->append(QString::fromAscii(str));

    // scroll to the bottom
    ui->textBrowser->verticalScrollBar()->setValue(
        ui->textBrowser->verticalScrollBar()->maximum());

    // if the file is a valid file descriptor, write to the file as well
    if(file != -1)
    {
        if(write(file,str,strlen(str)) == -1)
        {
            perror("failed on write");
        }
        if(write(file,"\n",1) == -1)
        {
            perror("failed on write");
        }
    }
}

//////////////////////////////////
// Host subclass implementation //
//////////////////////////////////

/**
 * [ServerWindow::onConnect description]
 *
 * @function   ServerWindow::onConnect
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::onConnect(int socket)
 *
 * @param      socket [description]
 */
void ServerWindow::onConnect(int socket)
{
    Host::onConnect(socket);

    // tell the new client about all currently connected clients
    Net::Message newClntMsg;
    newClntMsg.type = ADD_CLIENT;
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        QListWidgetItem* currListItem = client.value();

        newClntMsg.data = (void*)currListItem->text().toStdString().c_str();
        newClntMsg.len  = strlen((char*)newClntMsg.data)+1;
        send(socket,newClntMsg);
    }
}

/**
 * [ServerWindow::onMessage description]
 *
 * @function   ServerWindow::onMessage
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::onMessage(int socket, Net::Message msg)
 *
 * @param      socket [description]
 * @param      msg [description]
 */
void ServerWindow::onMessage(int socket, Net::Message msg)
{
    Host::onMessage(socket,msg);

    // switch on message type, and call handlers
    switch(msg.type)
    {
    case SHOW_MSG:
        onShowMessage(socket,(char*)msg.data);
        break;
    case CHECK_USR_NAME:
        onCheckUserName(socket,(char*)msg.data);
        break;
    }
}

/**
 * [ServerWindow::onDisconnect description]
 *
 * @function   ServerWindow::onDisconnect
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::onDisconnect(int socket, int remote)
 *
 * @param      socket [description]
 * @param      remote [description]
 */
void ServerWindow::onDisconnect(int socket, int remote)
{
    Host::onDisconnect(socket,remote);

    // remove the user from the list
    rmUserListEntry(socket);

    // iterate through remaining users, and tell them who left
    Net::Message rmClntMsg;
    rmClntMsg.type = RM_CLIENT;
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        int currSocket = client.key();

        rmClntMsg.data = &socket;
        rmClntMsg.len  = sizeof(socket);
        send(currSocket,rmClntMsg);
    }
}

//////////////////////////////////
// callbacks triggered from GUI //
//////////////////////////////////

/**
 * [ServerWindow::on_actionConnect_triggered description]
 *
 * @function   ServerWindow::on_actionConnect_triggered
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ServerWindow::on_actionConnect_triggered()
 */
void ServerWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked())
    {
        // put application into a disconnected state
        on_actionDisconnect_triggered();
    }
    else
    {
        if(startListeningRoutine(port) == 0)
        {
            // put application into a connected state
            ui->actionDisconnect->setEnabled(true);
            ui->actionSettings->setEnabled(false);
            appendText("started the server");
        }
        else
        {
            on_actionDisconnect_triggered();
            appendText("failed to start the server");
        }
    }
}

/**
 * [ServerWindow::on_actionDisconnect_triggered description]
 *
 * @function   ServerWindow::on_actionDisconnect_triggered
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ServerWindow::on_actionDisconnect_triggered()
 */
void ServerWindow::on_actionDisconnect_triggered()
{
    // put application into a disconnected state
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);
    stopListeningRoutine();
}

/**
 * [ServerWindow::on_actionSettings_triggered description]
 *
 * @function   ServerWindow::on_actionSettings_triggered
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ServerWindow::on_actionSettings_triggered()
 */
void ServerWindow::on_actionSettings_triggered()
{
    // put current settings into a structure
    Results savedResults;
    savedResults.port = port;
    savedResults.filePath = filePath;

    // create and show the settings dialog
    Dialog settings(this);
    settings.setData(savedResults);
    settings.exec();

    // retrieve the user's settings
    Results passed = settings.getResults();
    port     = passed.port;
    filePath = passed.filePath;

    // open the specified file for writing
    ::close(file);
    file = open(filePath.toStdString().c_str(),O_APPEND|O_CREAT|O_WRONLY,0777);
    if(file == -1)
    {
        perror("failed to open file");
    }
}

////////////////////////////////////////////////////////
// callbacks triggered from received network messages //
////////////////////////////////////////////////////////

/**
 * [ServerWindow::onShowMessage description]
 *
 * @function   ServerWindow::onShowMessage
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::onShowMessage(int socket, char* cstr)
 *
 * @param      socket [description]
 * @param      cstr [description]
 */
void ServerWindow::onShowMessage(int socket, char* cstr)
{
    // display the text
    appendText(cstr);

    // forward the received message to all other clients
    Net::Message msg;
    msg.type = SHOW_MSG;
    msg.data = (void*) cstr;
    msg.len  = strlen(cstr)+1;
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        int currSocket = client.key();
        if(currSocket != socket)
        {
            send(currSocket,msg);
        }
    }
}

/**
 * [ServerWindow::onCheckUserName description]
 *
 * @function   ServerWindow::onCheckUserName
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ServerWindow::onCheckUserName(int socket, char* cname)
 *
 * @param      socket [description]
 * @param      cname [description]
 */
void ServerWindow::onCheckUserName(int socket, char* cname)
{
    // iterate through names, and make sure there are no collisions & reassign name if needed
    QString name = cname;
    for (int s = 0; s < ui->listWidget->count(); ++s)
    {
        if (ui->listWidget->item(s)->text() == name) {

            name.append("1");
            s = -1;
        }
    }

    // send new name back to client
    Net::Message newNameMsg;
    newNameMsg.type = SET_USR_NAME;
    newNameMsg.data = (void*)name.toStdString().c_str();
    newNameMsg.len  = strlen((char*)newNameMsg.data)+1;
    send(socket,newNameMsg);

    // add client to client list
    addUserListEntry(socket,name);

    // tell all clients that a new client has connected
    Net::Message newClntMsg;
    newClntMsg.type = ADD_CLIENT;
    newClntMsg.data = (void*)name.toStdString().c_str();
    newClntMsg.len  = strlen((char*)newClntMsg.data)+1;
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        int currSocket = client.key();
        send(currSocket,newClntMsg);
    }
}
