#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T10:17:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
    dialog.cpp \
    clientwindow.cpp \
    Host.cpp \
    net_helper.cpp \
    select_helper.cpp

HEADERS  += \
    dialog.h \
    clientwindow.h \
    Host.h \
    Message.h \
    net_helper.h \
    protocol.h \
    select_helper.h

FORMS    += \
    dialog.ui \
    clientwindow.ui

RESOURCES += \
    theIcons.qrc \
    theactions.qrc

OTHER_FILES += \
    client.pro.user \

QMAKE_CXXFLAGS += -std=c++11
