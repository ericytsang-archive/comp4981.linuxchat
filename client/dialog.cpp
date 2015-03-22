#include "dialog.h"
#include "ui_dialog.h"

#include <sstream>

Results theResults;
int tempPort = 80;
QString tempName = "";
QString tempIP = "";
QString tempFilePath = "";

int prevPort = 80;
QString prevName = "";
QString prevIP = "";
QString prevFilePath = "";

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

Results Dialog::getResults(){

    return theResults;
}

void Dialog::setData(Results passedResults)
{
    prevName = passedResults.name;
    prevIP = passedResults.ip;
    prevPort = passedResults.port;
    prevFilePath = passedResults.filePath;

    std::stringstream sstm;
    sstm << prevPort;
    QString portString = sstm.str().c_str();

    ui->lineEdit->setText(prevName);
    ui->lineEdit_2->setText(prevIP);
    ui->lineEdit_3->setText(portString);
    ui->lineEdit_4->setText(prevFilePath);
}

void Dialog::on_lineEdit_textChanged(const QString &arg1)
{
    tempName = arg1;
}

void Dialog::on_lineEdit_2_textChanged(const QString &arg1)
{
     tempIP = arg1;
}

void Dialog::on_lineEdit_3_textChanged(const QString &arg1)
{
     tempPort = arg1.toInt();
}

void Dialog::on_lineEdit_4_textChanged(const QString &arg1)
{
     tempFilePath = arg1;
}

void Dialog::on_buttonBox_accepted()
{
    theResults.name = tempName;
    theResults.ip = tempIP;
    theResults.port = tempPort;
    theResults.filePath = tempFilePath;
}

void Dialog::on_buttonBox_rejected()
{
    theResults.port = prevPort;
    theResults.filePath = prevFilePath;
    theResults.name = prevName;
    theResults.ip = prevIP;
}
