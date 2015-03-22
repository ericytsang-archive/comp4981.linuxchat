#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "dialog.h"
#include <qmessagebox.h>

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
    filePath = "/";
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::append_window_text(QString string)
{
    ui->textBrowser->append(string);
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

void ClientWindow::onConnect(int socket)
{
    this->socket = socket;
    char buffer[1024];
    sprintf(buffer,"socket %d connected",socket);
    onShowMessage(buffer);
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
}

void ClientWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked()){
        ui->actionDisconnect->setEnabled(false);
        ui->actionSettings->setEnabled(true);
    } else {
        ui->actionDisconnect->setEnabled(true);
        ui->actionSettings->setEnabled(false);
    }
}

void ClientWindow::on_actionDisconnect_triggered()
{
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);
}

void ClientWindow::on_actionSettings_triggered()
{
    /* PopUp message before the settings*/
    QMessageBox pop;
     pop.setText("Display port: " + QString::number(port) + "\nIP: " + ip +"\nName: " + name +"\nFile Path: " + filePath);
    pop.exec();

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

    /* PopUp message after the settings*/
    pop.setText("Display port: " + QString::number(port) + "\nIP: " + ip +"\nName: " + name +"\nFile Path: " + filePath);
    pop.exec();


}

void ClientWindow::on_pushButton_clicked()
{
    Host::send(socket,ui->textEdit->toPlainText());
    ui->textBrowser->append(ui->textEdit->toPlainText());
    ui->textEdit->setText("");
}

void onAddClient(int socket, char* clientName)
{
    add_user(socket,QString::fromAscii(clientName));
}

void onRmClient(int socket)
{
    rm_user(socket);
}

void onShowMessage(char* message)
{
    ui->textBrowser->append(QString::fromAscii(message);

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

void onSetName(char* newUsername)
{
    name = QString::fromAscii(newUsername);
}
