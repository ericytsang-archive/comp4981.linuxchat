/********************************************************************************
** Form generated from reading UI file 'serverwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWINDOW_H
#define UI_SERVERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerWindow
{
public:
    QAction *actionSettings;
    QAction *actionDisconnect;
    QAction *actionConnect;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QLabel *label;
    QTextBrowser *textBrowser;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *ServerWindow)
    {
        if (ServerWindow->objectName().isEmpty())
            ServerWindow->setObjectName(QString::fromUtf8("ServerWindow"));
        ServerWindow->resize(404, 313);
        actionSettings = new QAction(ServerWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionDisconnect = new QAction(ServerWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionConnect = new QAction(ServerWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionConnect->setCheckable(true);
        centralWidget = new QWidget(ServerWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMinimumSize(QSize(114, 0));
        listWidget->setMaximumSize(QSize(182, 90000));
        listWidget->setSelectionMode(QAbstractItemView::NoSelection);

        gridLayout->addWidget(listWidget, 1, 0, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(182, 21));
        label->setMaximumSize(QSize(182, 21));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(textBrowser, 1, 1, 1, 1);

        ServerWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ServerWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ServerWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(ServerWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMovable(false);
        ServerWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        mainToolBar->addAction(actionConnect);
        mainToolBar->addAction(actionDisconnect);
        mainToolBar->addAction(actionSettings);

        retranslateUi(ServerWindow);

        QMetaObject::connectSlotsByName(ServerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ServerWindow)
    {
        ServerWindow->setWindowTitle(QApplication::translate("ServerWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("ServerWindow", "Settings", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("ServerWindow", "Disconnect", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("ServerWindow", "Connect", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ServerWindow", "Users Online:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServerWindow: public Ui_ServerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWINDOW_H
