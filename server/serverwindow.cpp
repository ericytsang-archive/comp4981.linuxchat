#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "dialog.h"
#include "protocol.h"
#include "Message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <qmessagebox.h>
#include <unistd.h>

int port = 7000;
int file = -1;
QString filePath = "/";


ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);


    /* Initial Settings */
    ui->actionDisconnect->setEnabled(false);
}

ServerWindow::~ServerWindow()
{
    file = ::close(file);
    delete ui;
}



void ServerWindow::append_window_text(QString string)
{
    ui->textBrowser->append(string);
}

void ServerWindow::add_user(int key, QString usrName)
{
    QListWidgetItem* li = new QListWidgetItem(usrName);
    lis.insert(key,li);
    ui->listWidget->addItem(li);
}

void ServerWindow::rm_user(int key)
{
    ui->listWidget->removeItemWidget(lis[key]);
    delete lis[key];
    lis.remove(key);
}

void ServerWindow::onConnect(int socket)
{
    char buffer[1024];
    sprintf(buffer,"socket %d connected",socket);
    appendText(buffer);

    // tell the new client about all connected clients
    Net::Message newClntMsg;
    newClntMsg.type = ADD_CLIENT;
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        int currSocket = client.key();
        QListWidgetItem* currListItem = client.value();

        newClntMsg.data = (void*)currListItem->text().toStdString().c_str();
        newClntMsg.len  = strlen((char*)newClntMsg.data);
        send(socket,newClntMsg);
    }
}

void ServerWindow::onMessage(int socket, Net::Message msg)
{
    Host::onMessage(socket,msg);
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

void ServerWindow::onDisconnect(int socket, int remote)
{
    char buffer[1024];
    sprintf(buffer,"socket %d disconnected by %s host",socket,remote?"remote":"local");
    appendText(buffer);

    // remove the user from the list
    rm_user(socket);

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

void ServerWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked()){
        ui->actionDisconnect->setEnabled(false);
        ui->actionSettings->setEnabled(true);
        stopListeningRoutine();
    } else {
        ui->actionDisconnect->setEnabled(true);
        ui->actionSettings->setEnabled(false);
        startListeningRoutine(port);
    }
}

void ServerWindow::on_actionDisconnect_triggered()
{
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);
    stopListeningRoutine();
}

void ServerWindow::on_actionSettings_triggered()
{
    Dialog settings(this);
    Results savedResults;
    savedResults.port = port;
    savedResults.filePath = filePath;

    settings.setData(savedResults);
    settings.exec();

    Results passed = settings.getResults();

    port = passed.port;
    filePath = passed.filePath;

    ::close(file);
    file = open(filePath.toStdString().c_str(),O_APPEND|O_CREAT|O_WRONLY,0777);
    if(file == -1)
    {
        perror("failed to open file");
    }

    /* PopUp message after the settings*/
    QMessageBox pop;
    pop.setText("Display port: " + QString::number(port) + "\nFile Path: " + filePath);
    pop.exec();
}

void ServerWindow::onShowMessage(int socket, char* cstr)
{
    // display the text
    appendText(cstr);

    // forward the received message to all other clients
    Net::Message msg;
    msg.type = SHOW_MSG;
    msg.data = (void*) cstr;
    msg.len  = strlen(cstr);
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        int currSocket = client.key();
        if(currSocket != socket)
        {
            send(currSocket,msg);
        }
    }
}

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
    newNameMsg.len  = strlen((char*)newNameMsg.data);
    send(socket,newNameMsg);

    // add client to client list
    add_user(socket,name);

    // tell all clients that a new client has connected
    Net::Message newClntMsg;
    newClntMsg.type = ADD_CLIENT;
    newClntMsg.data = (void*)name.toStdString().c_str();
    newClntMsg.len  = strlen((char*)newClntMsg.data);
    for(auto client = lis.begin(); client != lis.end(); ++client)
    {
        int currSocket = client.key();
        send(currSocket,newClntMsg);
    }
}

void ServerWindow::appendText(char* str)
{
    QString qstr = str;
    ui->textBrowser->append(qstr);

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
