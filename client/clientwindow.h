#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
    void append_window_text(QString string);
    void add_user(int key, QString usrName);
    void rm_user(int key);

private slots:
    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_actionSettings_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<int,QListWidgetItem*> lis;
};

#endif // CLIENTWINDOW_H
