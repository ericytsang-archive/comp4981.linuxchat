/**
 * [c  description]
 *
 * @sourceFile clientwindow.cpp
 *
 * @program    client
 *
 * @function   ClientWindow::ClientWindow(QWidget *parent)
 * @function   ClientWindow::~ClientWindow()
 * @function   void ClientWindow::addUserListEntry(int key, QString usrName)
 * @function   void ClientWindow::rmUserListEntry(int key)
 * @function   void ClientWindow::clearUserList()
 * @function   void ClientWindow::appendText(char* message)
 * @function   void ClientWindow::onConnect(int socket)
 * @function   void ClientWindow::onMessage(int socket, Net::Message msg)
 * @function   void ClientWindow::onDisconnect(int socket, int remote)
 * @function   void ClientWindow::on_actionConnect_triggered()
 * @function   void ClientWindow::on_actionDisconnect_triggered()
 * @function   void ClientWindow::on_actionSettings_triggered()
 * @function   void ClientWindow::on_pushButton_clicked()
 * @function   void ClientWindow::onAddClient(int socket, char* clientName)
 * @function   void ClientWindow::onRmClient(int socket)
 * @function   void ClientWindow::onSetName(char* newUsername)
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 */
#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "dialog.h"
#include "Message.h"
#include "protocol.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <qmessagebox.h>
#include <qscrollbar.h>
#include <sys/socket.h>

////////////////////////////////
// constructors & destructors //
////////////////////////////////

/**
 * [ClientWindow::ClientWindow description]
 *
 * @function   ClientWindow::ClientWindow
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  ClientWindow::ClientWindow(QWidget *parent)
 *
 * @param      parent [description]
 */
ClientWindow::ClientWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     // initial settings
    ui->actionDisconnect->setEnabled(false);

    // initialize instance variables
    port = 7000;
    file = -1;
    socket = -1;
    ip = "localhost";
    name = "user";
    displayName = "";
    filePath = "./test";
}

/**
 * [ClientWindow::ClientWindow description]
 *
 * @function   ClientWindow::~ClientWindow
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  ClientWindow::~ClientWindow()
 */
ClientWindow::~ClientWindow()
{
    delete ui;
}

//////////////////////
// public interface //
//////////////////////

/**
 * [ClientWindow::addUserListEntry description]
 *
 * @function   ClientWindow::addUserListEntry
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::addUserListEntry(int key, QString usrName)
 *
 * @param      key [description]
 * @param      usrName [description]
 */
void ClientWindow::addUserListEntry(int key, QString usrName)
{
    QListWidgetItem* li = new QListWidgetItem(usrName);
    lis.insert(key,li);
    ui->listWidget->addItem(li);
}

/**
 * [ClientWindow::rmUserListEntry description]
 *
 * @function   ClientWindow::rmUserListEntry
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::rmUserListEntry(int key)
 *
 * @param      key [description]
 */
void ClientWindow::rmUserListEntry(int key)
{
    ui->listWidget->removeItemWidget(lis[key]);
    delete lis[key];
    lis.remove(key);
}

/**
 * [ClientWindow::clearUserList description]
 *
 * @function   ClientWindow::clearUserList
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::clearUserList()
 */
void ClientWindow::clearUserList()
{
    ui->listWidget->clear();
    lis.clear();
}

/**
 * [ClientWindow::appendText description]
 *
 * @function   ClientWindow::appendText
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::appendText(char* message)
 *
 * @param      message [description]
 */
void ClientWindow::appendText(char* message)
{
    // append the passed text onto the screen
    ui->textBrowser->append(QString::fromAscii(message));

    // scroll to the bottom
    ui->textBrowser->verticalScrollBar()->setValue(
        ui->textBrowser->verticalScrollBar()->maximum());

    // if the file is a valid file descriptor, write to the file as well
    if(file != -1)
    {
        if(write(file,message,strlen(message)) == -1)
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
 * [ClientWindow::onConnect description]
 *
 * @function   ClientWindow::onConnect
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::onConnect(int socket)
 *
 * @param      socket [description]
 */
void ClientWindow::onConnect(int socket)
{
    Host::onConnect(socket);

    // keep a reference to the socket
    this->socket = socket;

    // send a message to the server, requesting a display name
    Net::Message msg;
    msg.type = CHECK_USR_NAME;
    msg.data = (void*)name.toStdString().c_str();
    msg.len  = strlen((char*)msg.data)+1;
    Host::send(socket,msg);
}

/**
 * [ClientWindow::onMessage description]
 *
 * @function   ClientWindow::onMessage
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::onMessage(int socket, Net::Message msg)
 *
 * @param      socket [description]
 * @param      msg [description]
 */
void ClientWindow::onMessage(int socket, Net::Message msg)
{
    Host::onMessage(socket,msg);

    // switch on message type, and call handlers
    switch(msg.type)
    {
    case ADD_CLIENT:
        onAddClient(socket,(char*)msg.data);
        break;
    case RM_CLIENT:
        onRmClient(socket);
        break;
    case SHOW_MSG:
        appendText((char*)msg.data);
        break;
    case SET_USR_NAME:
        onSetName((char*)msg.data);
        break;
    }
}

/**
 * [ClientWindow::onDisconnect description]
 *
 * @function   ClientWindow::onDisconnect
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::onDisconnect(int socket, int remote)
 *
 * @param      socket [description]
 * @param      remote [description]
 */
void ClientWindow::onDisconnect(int socket, int remote)
{
    Host::onDisconnect(socket,remote);

    // clear our user list
    clearUserList();
}

//////////////////////////////////
// callbacks triggered from GUI //
//////////////////////////////////

/**
 * [ClientWindow::on_actionConnect_triggered description]
 *
 * @function   ClientWindow::on_actionConnect_triggered
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ClientWindow::on_actionConnect_triggered()
 */
void ClientWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked())
    {
        // put application into a disconnected state
        on_actionDisconnect_triggered();
    }
    else
    {
        if(Host::connect((char*)ip.toStdString().c_str(),port) == 0)
        {
            // put application into a connected state
            ui->actionDisconnect->setEnabled(true);
            ui->actionSettings->setEnabled(false);
            appendText("connected with the server");
        }
        else
        {
            on_actionDisconnect_triggered();
            appendText("failed to connect with the server");
        }
    }
}

/**
 * [ClientWindow::on_actionDisconnect_triggered description]
 *
 * @function   ClientWindow::on_actionDisconnect_triggered
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ClientWindow::on_actionDisconnect_triggered()
 */
void ClientWindow::on_actionDisconnect_triggered()
{
    // put application into a disconnected state
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);
    Host::disconnect(this->socket);
}

/**
 * [ClientWindow::on_actionSettings_triggered description]
 *
 * @function   ClientWindow::on_actionSettings_triggered
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ClientWindow::on_actionSettings_triggered()
 */
void ClientWindow::on_actionSettings_triggered()
{
    // put current settings into a structure
    Results savedResults;
    savedResults.port     = port;
    savedResults.filePath = filePath;
    savedResults.ip       = ip;
    savedResults.name     = name;

    // create and show the settings dialog
    Dialog settings(this);
    settings.setData(savedResults);
    settings.exec();

    // retrieve the user's settings
    Results passed = settings.getResults();
    port     = passed.port;
    ip       = passed.ip;
    name     = passed.name;
    filePath = passed.filePath;

    // open the specified file for writing
    ::close(file);
    file = open(filePath.toStdString().c_str(),O_APPEND|O_CREAT|O_WRONLY,0777);
    if(file == -1)
    {
        perror("failed to open file");
    }
}

/**
 * [ClientWindow::on_pushButton_clicked description]
 *
 * @function   ClientWindow::on_pushButton_clicked
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ClientWindow::on_pushButton_clicked()
 */
void ClientWindow::on_pushButton_clicked()
{
    // prepare the chat message
    QString chatMsg = displayName+": "+ui->textEdit->toPlainText();

    // send a chat message over the network
    Net::Message msg;
    msg.type = SHOW_MSG;
    msg.data = (void*)chatMsg.toStdString().c_str();
    msg.len  = strlen((char*)msg.data)+1;
    Host::send(socket,msg);

    // update the GUI to display chat message, and clear chat input
    appendText((char*)msg.data);
    ui->textEdit->setText("");
}

////////////////////////////////////////////////////////
// callbacks triggered from received network messages //
////////////////////////////////////////////////////////

/**
 * [ClientWindow::onAddClient description]
 *
 * @function   ClientWindow::onAddClient
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::onAddClient(int socket, char* clientName)
 *
 * @param      socket [description]
 * @param      clientName [description]
 */
void ClientWindow::onAddClient(int socket, char* clientName)
{
    addUserListEntry(socket,QString::fromAscii(clientName));
}

/**
 * [ClientWindow::onRmClient description]
 *
 * @function   ClientWindow::onRmClient
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu & Eric Tsang
 *
 * @note       none
 *
 * @signature  void ClientWindow::onRmClient(int socket)
 *
 * @param      socket [description]
 */
void ClientWindow::onRmClient(int socket)
{
    rmUserListEntry(socket);
}

/**
 * Eric Tsang
 *
 * @function   ClientWindow::onSetName
 *
 * @date       2015-03-21
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @designer   Jonathan Chu
 *
 * @note       none
 *
 * @signature  void ClientWindow::onSetName(char* newUsername)
 *
 * @param      newUsername [description]
 */
void ClientWindow::onSetName(char* newUsername)
{
    // set user's display name
    displayName = QString::fromAscii(newUsername);

    // show our display name to the user
    char output[1024];
    sprintf(output,"You have joined the chat as %s.",newUsername);
    appendText(output);
}
