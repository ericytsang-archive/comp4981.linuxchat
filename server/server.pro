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
    dialog.cpp \
    Host.cpp \
    net_helper.cpp \
    select_helper.cpp

HEADERS  += serverwindow.h \
    dialog.h \
    Host.h \
    Message.h \
    net_helper.h \
    protocol.h \
    select_helper.h

FORMS    += serverwindow.ui \
    dialog.ui

RESOURCES +=

DISTFILES +=

QMAKE_CXXFLAGS += -std=c++11
