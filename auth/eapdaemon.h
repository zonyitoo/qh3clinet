#ifndef EAPDAEMON_H
#define EAPDAEMON_H

#include <QObject>
#include "eapauth.h"
#include <memory>
#include <QThread>

class AuthThread;

class EAPDaemon : public QObject
{
    Q_OBJECT
public:
    explicit EAPDaemon(QObject *parent = 0);
    
signals:
    void Message(const QString& msg);
    void Status(int statno);
public slots:
    void Login(const QVariantMap& userinfo);
    void Logoff();
    bool HasLogin();
    QString LoginUser();

private:
    std::shared_ptr<EAPAuth> authservice;
    std::auto_ptr<AuthThread> auththread;
    bool hasLogin;
};

class AuthThread : public QThread {
public:
    AuthThread(std::shared_ptr<EAPAuth> authservice, EAPDaemon *eapdaemon);
    void run();
    void reset_autoretry();
private:
    std::shared_ptr<EAPAuth> authservice;
    int autoretry_count;
    EAPDaemon *eapdaemon;
};

#endif // EAPDAEMON_H
