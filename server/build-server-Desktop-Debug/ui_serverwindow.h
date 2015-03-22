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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionSettings;
    QWidget *centralWidget;
    QTextBrowser *textBrowser;
    QLabel *label;
    QListWidget *listWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuBanana;

    void setupUi(QMainWindow *ServerWindow)
    {
        if (ServerWindow->objectName().isEmpty())
            ServerWindow->setObjectName(QString::fromUtf8("ServerWindow"));
        ServerWindow->resize(883, 597);
        actionConnect = new QAction(ServerWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionConnect->setCheckable(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/theIcon/theIcon/anohana_wallpaper_by_slydog0905-d4d02e7.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon);
        actionDisconnect = new QAction(ServerWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/theIcon/theIcon/bad_piggy_by_mikeinel-d5lz778.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon1);
        actionSettings = new QAction(ServerWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/theIcon/theIcon/sandshrew_wallpaper_by_dragonsdenda-d5zmlri.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon2);
        centralWidget = new QWidget(ServerWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(190, 20, 671, 501));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 131, 20));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(20, 50, 151, 471));
        listWidget->setSelectionMode(QAbstractItemView::NoSelection);
        ServerWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ServerWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ServerWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ServerWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ServerWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(ServerWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 883, 21));
        menuBanana = new QMenu(menuBar);
        menuBanana->setObjectName(QString::fromUtf8("menuBanana"));
        ServerWindow->setMenuBar(menuBar);

        mainToolBar->addAction(actionConnect);
        mainToolBar->addAction(actionDisconnect);
        mainToolBar->addAction(actionSettings);
        menuBar->addAction(menuBanana->menuAction());
        menuBanana->addSeparator();
        menuBanana->addAction(actionConnect);
        menuBanana->addAction(actionDisconnect);
        menuBanana->addAction(actionSettings);

        retranslateUi(ServerWindow);

        QMetaObject::connectSlotsByName(ServerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ServerWindow)
    {
        ServerWindow->setWindowTitle(QApplication::translate("ServerWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("ServerWindow", "Connect", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("ServerWindow", "Disconnect", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("ServerWindow", "Settings", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ServerWindow", "Users Online:", 0, QApplication::UnicodeUTF8));
        menuBanana->setTitle(QApplication::translate("ServerWindow", "Options", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServerWindow: public Ui_ServerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWINDOW_H
