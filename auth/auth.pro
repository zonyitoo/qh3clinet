#-------------------------------------------------
#
# Project created by QtCreator 2013-05-17T15:16:48
#
#-------------------------------------------------

QT       += core dbus

QT       -= gui

TARGET = qh3client-daemon
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    eapdaemonadapter.cpp \
    eapdaemon.cpp \
    eapauth.cpp

OTHER_FILES += \
    com.qh3client.EAPDaemon.conf \
    com.qh3client.EAPDaemon.service

HEADERS += \
    eapdaemonadapter.h \
    eapdaemon.h \
    eapauth.h \
    eapdef.h
