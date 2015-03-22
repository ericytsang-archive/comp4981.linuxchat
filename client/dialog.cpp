/**
 * [ovi description]
 *
 * @sourceFile dialog.cpp
 *
 * @program    client
 *
 * @function   Dialog::Dialog(QWidget *parent)
 * @function   Dialog::Dialog()
 * @function   Results Dialog::getResults()
 * @function   void Dialog::setData(Results passedResults)
 * @function   void Dialog::on_buttonBox_accepted()
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
#include "dialog.h"
#include "ui_dialog.h"

#include <sstream>

////////////////////////////////
// constructors & destructors //
////////////////////////////////

/**
 * [Dialog::Dialog description]
 *
 * @function   Dialog::Dialog
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
 * @signature  Dialog::Dialog(QWidget *parent)
 *
 * @param      parent [description]
 */
Dialog::Dialog(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

/**
 * [Dialog::Dialog description]
 *
 * @function   Dialog::Dialog
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
 * @signature  Dialog::Dialog()
 */
Dialog::~Dialog()
{
    delete ui;
}

//////////////////////
// public interface //
//////////////////////

/**
 * [Dialog::getResults description]
 *
 * @function   Dialog::getResults
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
 * @signature  Results Dialog::getResults()
 *
 * @return     [file_header] [class_header] [description]
 */
Results Dialog::getResults()
{
    return results;
}

/**
 * [Dialog::setData description]
 *
 * @function   Dialog::setData
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
 * @signature  void Dialog::setData(Results passedResults)
 *
 * @param      passedResults [description]
 */
void Dialog::setData(Results passedResults)
{
    // save the results in case the user presses cancel
    results.name     = passedResults.name;
    results.ip       = passedResults.ip;
    results.port     = passedResults.port;
    results.filePath = passedResults.filePath;

    // convert port number to string...
    std::stringstream sstm;
    sstm << results.port;
    QString portString = sstm.str().c_str();

    // st the texts of the dialog with values from passedResults
    ui->lineEdit  ->setText(results.name);
    ui->lineEdit_2->setText(results.ip);
    ui->lineEdit_3->setText(portString);
    ui->lineEdit_4->setText(results.filePath);
}

///////////////////
// GUI callbacks //
///////////////////

/**
 * [Dialog::on_buttonBox_accepted description]
 *
 * @function   Dialog::on_buttonBox_accepted
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
 * @signature  void Dialog::on_buttonBox_accepted()
 */
void Dialog::on_buttonBox_accepted()
{
    results.name     = ui->lineEdit  ->text();
    results.ip       = ui->lineEdit_2->text();
    results.port     = ui->lineEdit_3->text().toInt();
    results.filePath = ui->lineEdit_4->text();
}
