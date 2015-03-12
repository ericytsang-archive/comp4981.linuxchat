#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

struct Results {
    QString name;
    int port;
    QString ip;
    QString filePath;
};


class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    Results getResults();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
