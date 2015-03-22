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
    void appendText(char* message);

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
    void onSetName(char* newUsername);
    Ui::MainWindow *ui;
    /**
     * port number to connect to.
     */
    int port;
    /**
     * file descriptor to a file to record chat history to.
     */
    int file;
    /**
     * socket that's connected with the server.
     */
    int socket;
    /**
     * name of the remote host.
     */
    QString ip;
    /**
     * name that we want to be seen as in the chat room.
     */
    QString name;
    /**
     * the actual name we use in the chat room, because of possibility of name
     *   collisions.
     */
    QString displayName;
    /**
     * path to file to record chat history to.
     */
    QString filePath;
    /**
     * maps sockets to list items on the user list GUI widget.
     */
    QMap<int,QListWidgetItem*> lis;
};

#endif // CLIENTWINDOW_H
