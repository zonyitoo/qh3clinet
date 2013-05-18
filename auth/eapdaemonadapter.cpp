/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c EAPDaemonAdapter -a eapdaemonadapter.h:eapdaemonadapter.cpp com.qh3client.EAPDaemon.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "eapdaemonadapter.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class EAPDaemonAdapter
 */

EAPDaemonAdapter::EAPDaemonAdapter(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

EAPDaemonAdapter::~EAPDaemonAdapter()
{
    // destructor
}

bool EAPDaemonAdapter::HasLogin()
{
    // handle method call com.qh3client.EAPDaemon.HasLogin
    bool out0;
    QMetaObject::invokeMethod(parent(), "HasLogin", Q_RETURN_ARG(bool, out0));
    return out0;
}

void EAPDaemonAdapter::Login(const QVariantMap &userinfo)
{
    // handle method call com.qh3client.EAPDaemon.Login
    QMetaObject::invokeMethod(parent(), "Login", Q_ARG(QVariantMap, userinfo));
}

QString EAPDaemonAdapter::LoginUser()
{
    // handle method call com.qh3client.EAPDaemon.LoginUser
    QString out0;
    QMetaObject::invokeMethod(parent(), "LoginUser", Q_RETURN_ARG(QString, out0));
    return out0;
}

void EAPDaemonAdapter::Logoff()
{
    // handle method call com.qh3client.EAPDaemon.Logoff
    QMetaObject::invokeMethod(parent(), "Logoff");
}
