#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "dialog.h"
#include <qmessagebox.h>



int port = 80;
QString filePath = "/";


ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);


    /* Initial Settings */
    ui->actionDisconnect->setEnabled(false);

    add_user(1,"heyy1");
    add_user(2,"heyy2");
    add_user(3,"heyy3");
    add_user(4,"heyy4");
    add_user(5,"heyy5");
    rm_user(3);
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

void ServerWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked()){
        ui->actionDisconnect->setEnabled(false);
        ui->actionSettings->setEnabled(true);
    } else {
        ui->actionDisconnect->setEnabled(true);
        ui->actionSettings->setEnabled(false);
    }
}

void ServerWindow::on_actionDisconnect_triggered()
{
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);

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
