/**
 * the dialog that creates a window for the setting for the
 * server and saves the data to be applied by the server window
 *
 * @sourceFile dialog.cpp
 *
 * @program    server
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
 * the constructor that creates the dialog window
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
 * @param      parent
 */
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

/**
 * the destructor for the dialog window
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
 * function used by server window to obtain data
 * from the dialog window saved on a struct
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
 * @return     results struct containing the data from the settings
 */
Results Dialog::getResults()
{
    return results;
}

/**
 * sets the data of the struct that contains the information
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
 * @param      passedResults the previous setting
 */
void Dialog::setData(Results passedResults)
{
    // save the results in case the user presses cancel
    results.port     = passedResults.port;
    results.filePath = passedResults.filePath;

    // convert port number to string...
    std::stringstream sstm;
    sstm << results.port;
    QString portString = sstm.str().c_str();

    // set the texts of the dialog with values from passedResults
    ui->lineEdit  ->setText(portString);
    ui->lineEdit_2->setText(results.filePath);
}

///////////////////
// GUI callbacks //
///////////////////

/**
 * saves the changes in settings once the accept button is pressed
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
    results.port     = ui->lineEdit  ->text().toInt();
    results.filePath = ui->lineEdit_2->text();
}
