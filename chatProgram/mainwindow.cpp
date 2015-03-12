#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <qmessagebox.h>


int port = 80;
QString ip = "localhost";
QString name = "waifu00";
QString filePath = "/";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /* Initial Settings */
    ui->actionDisconnect->setEnabled(false);


    //testing only ~ have to move this later on

     ui->listWidget->addItem("Eric");
     ui->listWidget->setStyleSheet( "QListWidget::item { border-bottom: 1px solid red; }" );

    /* Populates the list widget  */
    for(int i= 1; i < 10; i++)
    {

        ui->listWidget->addItem("waifu number " + QString::number(i));
        if (i == 1)
            ui->listWidget->setCurrentRow(0);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    if (!ui->actionConnect->isChecked()){
        ui->actionDisconnect->setEnabled(false);
        ui->actionSettings->setEnabled(true);
    } else {
        ui->actionDisconnect->setEnabled(true);
        ui->actionSettings->setEnabled(false);
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    ui->actionConnect->setChecked(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSettings->setEnabled(true);

}

void MainWindow::on_actionSettings_triggered()
{



    /* PopUp message before the settings*/
    QMessageBox pop;
    pop.setText("Display port: " + QString::number(port) + "\nIP: " + ip +"\nName: " + name +"\nFile Path: " + filePath);
    pop.exec();

    Dialog settings(this);
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

void MainWindow::on_pushButton_clicked()
{

    ui->textBrowser->append(ui->textEdit->toPlainText());
    ui->textEdit->setText("");
}
