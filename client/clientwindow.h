/**
 * [void description]
 *
 * @sourceFile clientwindow.h
 *
 * @program    client
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
#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "Host.h"

#include <QMainWindow>
#include <QListWidgetItem>

//////////////////////////
// forward declarations //
//////////////////////////

namespace Net
{
class Host;
struct Message;
}

namespace Ui {
class MainWindow;
}

/**
 * the {ClientWindow} handles GUI events, updates the GUI as needed, and lets
 *   the user interact with the networking calls.
 */
class ClientWindow : public QMainWindow, public Net::Host
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
    void addUserListEntry(int key, QString usrName);
    void rmUserListEntry(int key);
    void clearUserList();

protected:
    virtual void onConnect(int socket);
    virtual void onMessage(int socket, Net::Message msg);
    virtual void onDisconnect(int socket, int remote);

private slots:
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSettings_triggered();
    void on_pushButton_clicked();

private:
    void onAddClient(int socket, char* clientName);
    void onRmClient(int socket);
    void onShowMessage(char* message);
    void onSetName(char* newUsername);
    int port;
    int file;
    int socket;
    QString ip;
    QString name;
    QString displayName;
    QString filePath;
    Ui::MainWindow *ui;
    QMap<int,QListWidgetItem*> lis;
};

#endif // CLIENTWINDOW_H
