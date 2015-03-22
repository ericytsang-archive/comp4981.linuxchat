#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "dialog.h"
#include <qmessagebox.h>


int port = 80;
QString ip = "localhost";
QString name = "waifu00";
QString filePath = "/";

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    //settings.setData(savedResults);
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

    ui->textBrowser->append(ui->textEdit->toPlainText());
    ui->textEdit->setText("");
}
