#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T09:59:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        serverwindow.cpp \
    dialog.cpp

HEADERS  += serverwindow.h \
    dialog.h

FORMS    += serverwindow.ui \
    dialog.ui

RESOURCES += \
    theicons.qrc

DISTFILES +=
