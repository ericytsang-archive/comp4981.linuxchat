#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "dialog.h"
#include "protocol.h"
#include "Message.h"
#include <qmessagebox.h>



int port = 7000;
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
    sprintf(buffer,"socket %d connected\n",socket);
    appendText(buffer);


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
    sprintf(buffer,"socket %d disconnected by %s host\n",socket,remote?"remote":"local");
    appendText(buffer);
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

    /* PopUp message before the settings*/
    QMessageBox pop;
    pop.setText("Display port: " + QString::number(port) + "\nFile Path: " + filePath);
    pop.exec();

    Dialog settings(this);
    Results savedResults;
    savedResults.port = port;
    savedResults.filePath = filePath;

    settings.setData(savedResults);
    settings.exec();

    Results passed = settings.getResults();

    port = passed.port;
    filePath = passed.filePath;


    /* PopUp message after the settings*/
    pop.setText("Display port: " + QString::number(port) + "\nFile Path: " + filePath);
    pop.exec();
}

void ServerWindow::onShowMessage(int socket, char* cstr)
{
    appendText(cstr);
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
}
