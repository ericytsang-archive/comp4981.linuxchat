#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T14:30:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatProgram
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    theactions.qrc
