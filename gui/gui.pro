#-------------------------------------------------
#
# Project created by QtCreator 2013-05-16T13:08:46
#
#-------------------------------------------------

QT       += core gui network dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qh3client
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwidget.cpp \
    eapdaemoninterface.cpp

HEADERS  += mainwidget.h \
    eapdaemoninterface.h

FORMS    += mainwidget.ui

RESOURCES += \
    pics.qrc
