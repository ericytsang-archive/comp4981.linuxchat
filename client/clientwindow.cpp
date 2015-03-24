/**
 * The client chat window will be created as well as
 * all the functions that it deals with it.
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
 *client window constructor that sets some gui behaviorus as
 * well as the default settings for the client.
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
 * @param      parent
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

    // connect signals to slots
    qRegisterMetaType<Net::Message>();
    qRegisterMetaType<int>();
    QObject::connect(this,SIGNAL(sigConnect(int)),
                     this,SLOT(slot_connect(int)));
    QObject::connect(this,SIGNAL(sigMessage(int,Net::Message)),
                     this,SLOT(slot_message(int,Net::Message)));
    QObject::connect(this,SIGNAL(sigDisconnect(int,int)),
                     this,SLOT(slot_disconnect(int,int)));
}

/**
 * The clientwindow destructor
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
 * Adds a user to the qlistwidget, which is the list of
 * users that are online
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
 * @param      key the id of the user that is to be added
 * @param      usrName the name of the user that is being added
 */
void ClientWindow::addUserListEntry(int key, QString usrName)
{
    QListWidgetItem* li = new QListWidgetItem(usrName);
    lis.insert(key,li);
    ui->listWidget->addItem(li);
}

/**
 * Removes a user from the qlistwidget
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
 * @param      key the id of the user to be removed
 */
void ClientWindow::rmUserListEntry(int key)
{
    ui->listWidget->removeItemWidget(lis[key]);
    delete lis[key];
    lis.remove(key);
}

/**
 * removes all entries from the qlistwidget
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
 * Appends the text that is being given to the display
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
 * @param      message message to be appended
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
 * Callback that is called when a new connection is established
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
 * @param      socket the socket that is being connected to
 */
void ClientWindow::onConnect(int socket)
{
    emit(sigConnect(socket));
}

/**
 * Callback that is called when a new message is received
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
 * @param      socket socket which message was sent from
 *             msg the message
 */
void ClientWindow::onMessage(int socket, Net::Message msg)
{
    emit(sigMessage(socket,msg));
}

/**
 * Callback that is called when it disconnects
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
 * @param      socket the socket that is to be disconnnected
 *             remote a boolean where its zero or one depending on who terminated the connection
 */
void ClientWindow::onDisconnect(int socket, int remote)
{
    emit(sigDisconnect(socket,remote));
}

//////////////////////////////////
// callbacks triggered from GUI //
//////////////////////////////////

/**
 * Invoked by onConnect
 *
 * @function   ClientWindow::slot_connect
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
 * @signature  void ClientWindow::slot_connect(int socket)
 *
 * @param      socket the socket
 */
void ClientWindow::slot_connect(int socket)
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
 * invoked by onMessage
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
 * @param      socket the socket
 * @param      msg the message sent
 */
void ClientWindow::slot_message(int socket, Net::Message msg)
{
    Host::onMessage(socket,msg);

    // switch on message type, and call handlers
    switch(msg.type)
    {
    case ADD_CLIENT:
        onAddClient(((int*)msg.data)[0],&((char*)msg.data)[4]);
        break;
    case RM_CLIENT:
        onRmClient(((int*)msg.data)[0]);
        break;
    case SHOW_MSG:
        appendText((char*)msg.data);
        break;
    case SET_USR_NAME:
        onSetName((char*)msg.data);
        break;
    }

    free(msg.data);
}

/**
 * invoked by onDisconnect
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
 * @param      socket the socket
 * @param      remote boolean where its zero or one depending on who terminated the connection
 */
void ClientWindow::slot_disconnect(int socket, int remote)
{
    Host::onDisconnect(socket,remote);

    // clear our user list
    clearUserList();
}

//////////////////////////////////
// callbacks triggered from GUI //
//////////////////////////////////

/**
 * The connect button sets all the other tabs to their
 * corresponding state and connects to the server or disconnects
 * if it is toggled
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
 * The disconnect button disconnects from the server and updates
 * all the states.
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
 * Pops up the new window which is a dialog that contains
 * the settings for the connection.
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
 * The send button which will gather the message to be sent and writes it to the
 * screen and send the same data with the username to the server to update
 * the other clients as well as the server
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
 * add a client to the server
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
 * @param      socket the socket being used
 * @param      clientName the name of the client that was specified
 */
void ClientWindow::onAddClient(int socket, char* clientName)
{
    fprintf(stderr,"add client: %d\n",socket);
    addUserListEntry(socket,QString::fromAscii(clientName));
}

/**
 * removes a client from the server.
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
 * @param      socket the socket that is being used by the client
 */
void ClientWindow::onRmClient(int socket)
{
    fprintf(stderr,"rm client: %d\n",socket);
    rmUserListEntry(socket);
}

/**
 * sets the user's display name as well as display a message
 *  that will tell the user that he has connected successfully
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
 * @param      newUsername the username for the user
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
