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
#include <sys/socket.h>

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Initial Settings */
    ui->actionDisconnect->setEnabled(false);

    // initialize instance variables
    port = 80;
    file = -1;
    socket = -1;
    ip = "localhost";
    name = "waifu00";
    displayName = "";
    filePath = "/";
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::add_user(int key, QString usrName)
{
    QListWidgetItem* li = new QListWidgetItem(usrName);
    lis.insert(key,li);
    ui->listWidget->addItem(li);
}

void ClientWindow::rm_user(int key)
{
    ui->listWidget->removeItemWidget(lis[key]);
    delete lis[key];
    lis.remove(key);
}

void ClientWindow::clr_users()
{
    ui->listWidget->clear();
    lis.clear();
}

void ClientWindow::onConnect(int socket)
{
    this->socket = socket;
    char buffer[1024];
    sprintf(buffer,"socket %d connected",socket);
    onShowMessage(buffer);

    Net::Message msg;
    msg.type = CHECK_USR_NAME;
    msg.data = (void*)name.toStdString().c_str();
    msg.len  = strlen((char*)msg.data);
    Host::send(socket,msg);
}

void ClientWindow::onMessage(int socket, Net::Message msg)
{
    Host::onMessage(socket,msg);
    switch(msg.type)
    {
    case ADD_CLIENT:
        onAddClient(socket,(char*)msg.data);
        break;
    case RM_CLIENT:
        onRmClient(socket);
        break;
    case SHOW_MSG:
        onShowMessage((char*)msg.data);
        break;
    case SET_USR_NAME:
        onSetName((char*)msg.data);
        break;
    }
}

void ClientWindow::onDisconnect(int socket, int remote)
{
    char buffer[1024];
    sprintf(buffer,"socket %d disconnected by %s host",socket,remote?"remote":"local");
    onShowMessage(buffer);
    clr_users();
}

void ClientWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked()){
        on_actionDisconnect_triggered();
    } else {
        ui->actionDisconnect->setEnabled(true);
        ui->actionSettings->setEnabled(false);
        Host::connect((char*)ip.toStdString().c_str(),port);
    }
}

void ClientWindow::on_actionDisconnect_triggered()
{
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);
    ::shutdown(this->socket,SHUT_RDWR);
}

void ClientWindow::on_actionSettings_triggered()
{
    Dialog settings(this);
    Results savedResults;
    savedResults.port = port;
    savedResults.filePath = filePath;
    savedResults.ip = ip;
    savedResults.name = name;

    settings.setData(savedResults);
    settings.exec();

    Results passed = settings.getResults();

    port = passed.port;
    ip = passed.ip;
    name = passed.name;
    filePath = passed.filePath;

    ::close(file);
    file = open(filePath.toStdString().c_str(),O_APPEND|O_CREAT|O_WRONLY,0777);
    if(file == -1)
    {
        perror("failed to open file");
    }
}

void ClientWindow::on_pushButton_clicked()
{
    QString chatMsg = displayName+": "+ui->textEdit->toPlainText();

    Net::Message msg;
    msg.type = SHOW_MSG;
    msg.data = (void*)chatMsg.toStdString().c_str();
    msg.len  = strlen((char*)msg.data);
    Host::send(socket,msg);

    onShowMessage((char*)msg.data);
    ui->textEdit->setText("");
}

void ClientWindow::onAddClient(int socket, char* clientName)
{
    add_user(socket,QString::fromAscii(clientName));
}

void ClientWindow::onRmClient(int socket)
{
    rm_user(socket);
}

void ClientWindow::onShowMessage(char* message)
{
    ui->textBrowser->append(QString::fromAscii(message));

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

void ClientWindow::onSetName(char* newUsername)
{
    displayName = QString::fromAscii(newUsername);
}
