#include "dialog.h"
#include "ui_dialog.h"

Results theResults;

int tempPort = 80;
QString tempFilePath = "/";
int prevPort = 0;
QString prevFilePath = "/";


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

void Dialog::setData(Results passedResults)
{
    prevPort = passedResults.port;

    prevFilePath = passedResults.filePath;

}

Results Dialog::getResults()
{
    return theResults;
}

void Dialog::on_lineEdit_textChanged(const QString &arg1)
{
     tempPort = arg1.toInt();
}

void Dialog::on_lineEdit_2_textChanged(const QString &arg1)
{
     tempFilePath = arg1;
}

void Dialog::on_buttonBox_accepted()
{
    theResults.port = tempPort;
    theResults.filePath = tempFilePath;
}


void Dialog::on_buttonBox_rejected()
{
    theResults.port = prevPort;
    theResults.filePath = prevFilePath;
}
