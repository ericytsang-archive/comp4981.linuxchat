/**
 * [void description]
 *
 * @sourceFile serverwindow.h
 *
 * @program    server
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
#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include "Host.h"
#include "Message.h"

#include <QMainWindow>
#include <QListWidgetItem>

namespace Net {
class Host;
}

namespace Ui {
class ServerWindow;
}

/**
 * the {ServerWindow} handles GUI events, updates the GUI as needed, and lets
 *   the user interact with the networking calls.
 */
class ServerWindow : public QMainWindow, public Net::Host
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();
    void addUserListEntry(int key, QString usrName);
    void rmUserListEntry(int key);

protected:
    virtual void onConnect(int socket);
    virtual void onMessage(int socket, Net::Message msg);
    virtual void onDisconnect(int socket, int remote);

signals:
    void sigConnect(int socket);
    void sigMessage(int socket, Net::Message msg);
    void sigDisconnect(int socket, int remote);

public slots:
    void slot_connect(int socket);
    void slot_message(int socket, Net::Message msg);
    void slot_disconnect(int socket, int remote);
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSettings_triggered();

private:
    void onShowMessage(int socket, char* cstr);
    void onCheckUserName(int socket, char* name);
    void appendText(char* str);
    Ui::ServerWindow *ui;
    /**
     * maps sockets to list items on the user list GUI widget.
     */
    QMap<int,QListWidgetItem*> lis;
    /**
     * port number to listen on for connections.
     */
    int port;
    /**
     * file descriptor to a file to record chat history to.
     */
    int file;
    /**
     * path to file to record chat history to.
     */
    QString filePath;


};

#endif // SERVERWINDOW_H
