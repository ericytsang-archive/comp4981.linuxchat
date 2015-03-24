/**
 * the dialog header that contains all related
 * to the settings tab
 *
 * @sourceFile dialog.h
 *
 * @program    server
 *
 * @date       2015-03-22
 *
 * @revision   none
 *
 * @designer   Jonathan Chu
 *
 * @programmer Jonathan Chu
 *
 * @note       none
 */
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

/**
 * holds the configuration results indicated by the user, once they are done
 *   with the dialog box.
 */
struct Results {
    int port;
    QString filePath;
};

/**
 * dialog box, used to get the user's preferences.
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    Results getResults();
    void setData(Results passedResults);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    Results results;
};

#endif // DIALOG_H
