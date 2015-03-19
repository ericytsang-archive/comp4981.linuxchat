#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include "Host.h"

#include <QMainWindow>
#include <QListWidgetItem>

namespace Net {
class Host;
}

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow, public Net::Host
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();
    void append_window_text(QString string);
    void add_user(int key, QString usrName);
    void rm_user(int key);

protected:
    void onConnect(int socket);
    void onMessage(int socket, Net::Message msg);
    void onDisconnect(int socket, int remote);

private slots:
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSettings_triggered();

private:
    void onShowMessage(int socket, char* cstr);
    void onCheckUserName(int socket, char* name);
    void appendText(char* str);
    Ui::ServerWindow *ui;
    QMap<int,QListWidgetItem*> lis;


};

#endif // SERVERWINDOW_H
