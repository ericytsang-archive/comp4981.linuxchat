#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class ClientWindow : public QMainWindow, public Net::Host
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
    void append_window_text(QString string);
    void add_user(int key, QString usrName);
    void rm_user(int key);

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
    void onAddClient(char* clientName);
    void onRmClient(char* clientName);
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
