#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();
    void append_window_text(QString string);
    void add_user(int key, QString usrName);
    void rm_user(int key);

private slots:
    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_actionSettings_triggered();

private:
    Ui::ServerWindow *ui;
    QMap<int,QListWidgetItem*> lis;


};

#endif // SERVERWINDOW_H
