/**
 * The dialog that will be used as
 * the settings in the chat program.
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
 * The dialog constructor that creates the window
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
 * The dialog destructor that cleans up
 * when the window is closed or done with.
 *
 * @function   Dialog::~Dialog
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
 * @signature  Dialog::~Dialog()
 */
Dialog::~Dialog()
{
    delete ui;
}

//////////////////////
// public interface //
//////////////////////

/**
 * this function is used by the main window in order
 * to retrieve the information that was collected
 * by the settings.
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
 * @return     resuts the struc containing all the data important for the main window
 */
Results Dialog::getResults()
{
    return results;
}

/**
 * Sets the data that is being passed onto the settings so that
 * the previous settings are persistent.
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
 * @param      passedResults the previous settings
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

    // set the texts of the dialog with values from passedResults
    ui->lineEdit  ->setText(results.name);
    ui->lineEdit_2->setText(results.ip);
    ui->lineEdit_3->setText(portString);
    ui->lineEdit_4->setText(results.filePath);
}

///////////////////
// GUI callbacks //
///////////////////

/**
 * The behaviour that is required when the
 * confirmation button has been pressed.
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
    // put text box contents into result structure
    results.name     = ui->lineEdit  ->text();
    results.ip       = ui->lineEdit_2->text();
    results.port     = ui->lineEdit_3->text().toInt();
    results.filePath = ui->lineEdit_4->text();
}
