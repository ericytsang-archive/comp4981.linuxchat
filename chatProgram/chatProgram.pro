#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T10:17:37
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
    theIcons.qrc \
    theactions.qrc

OTHER_FILES += \
    chatProgram.pro.user \
