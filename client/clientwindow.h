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

class ClientWindow : public QMainWindow, public Net::Host
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
    void add_user(int key, QString usrName);
    void rm_user(int key);
    void clr_users();

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
    QString filePath;
    Ui::MainWindow *ui;
    QMap<int,QListWidgetItem*> lis;
};

#endif // CLIENTWINDOW_H
