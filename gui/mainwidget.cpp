#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QtNetwork/QNetworkInterface>
#include <QTimer>

#include "../auth/eapdef.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    settings(new QSettings("qh3client")),
    eapdaemonif(new EAPDaemonInterface("com.qh3client.EAPDaemon", "/",
                                       QDBusConnection::systemBus(), 0)),
    hasLogin(false)
{
    ui->setupUi(this);
    ui->statusPlainTextEdit->setHidden(true);

    QList<QNetworkInterface> ifacelist = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : ifacelist)
        ui->ifaceComboBox->addItem(iface.name());

    QStringList groups = settings->childGroups();
    for (const QString& gname : groups)
        ui->nameComboBox->addItem(gname);

    connect(eapdaemonif.get(), &EAPDaemonInterface::Message, [this] (const QString& msg) {
        ui->statusPlainTextEdit->appendHtml("<font color=red>" + msg + "</font>");
    });

    connect(eapdaemonif.get(), &EAPDaemonInterface::Status, [this] (int statno) {
        switch (statno) {
        case EAPAUTH_EAP_SUCCESS:
            hasLogin = true;
            ui->submitPushButton->setEnabled(true);
            ui->submitPushButton->setText(tr("下线"));

            settings->beginGroup(ui->nameComboBox->currentText().trimmed());
            settings->setValue("password", ui->pwdLineEdit->text().trimmed());
            settings->setValue("iface", ui->ifaceComboBox->currentText());
            settings->endGroup();

            ui->statusPlainTextEdit->appendPlainText(tr("登录成功"));

            QTimer::singleShot(3000, this, SLOT(timer_hide_status()));
            break;
        case EAPAUTH_EAP_FAILURE:
            unfreeze();
            if (hasLogin)
                ui->statusPlainTextEdit->appendHtml(tr("<font color=blue>认证失败</font>"));
            else
                ui->statusPlainTextEdit->appendHtml(tr("<font color=blue>下线成功</font>"));

            ui->submitPushButton->setText(tr("登录"));
            animShowStat();
            QTimer::singleShot(3000, this, SLOT(timer_hide_status()));
            break;
        case EAPAUTH_AUTH_START:
            ui->statusPlainTextEdit->appendPlainText(tr("开始认证 [")
                            + ui->nameComboBox->currentText().trimmed()
                            + "]");
            break;
        }
    });

    // If HasLogin
    if (eapdaemonif->isValid() && eapdaemonif->HasLogin()) {
        QString uname = eapdaemonif->LoginUser();
        if (settings->contains(uname + "/password"))
        {
            settings->beginGroup(uname);
            ui->pwdLineEdit->setText(settings->value("password").toString());
            int ifaceind = ui->ifaceComboBox->findText(settings->value("iface").toString());
            if (ifaceind != -1)
                ui->ifaceComboBox->setCurrentIndex(ifaceind);
            settings->endGroup();
            int nameind = ui->nameComboBox->findText(uname);
            if (nameind != -1)
                ui->nameComboBox->setCurrentIndex(nameind);
        }

        ui->statusPlainTextEdit->appendPlainText("[" + uname + tr("] 已登录"));
        ui->submitPushButton->setText(tr("下线"));
        hasLogin = true;
        freeze();
        ui->submitPushButton->setEnabled(true);
    }
}

MainWidget::~MainWidget()
{
    delete ui;
    delete settings;
}

void MainWidget::closeEvent(QCloseEvent *) {
    qApp->quit();
}

void MainWidget::on_statPushButton_clicked()
{
    if (ui->statusPlainTextEdit->isHidden())
        this->animShowStat();
    else
        this->animHideStat();
}

void MainWidget::animHideStat()
{
    ui->statPushButton->setText(tr("显示"));
    QPropertyAnimation *wAnim = new QPropertyAnimation(this, "geometry");
    wAnim->setStartValue(this->geometry());
    QRect endval = this->geometry();
    endval.setHeight(267);
    wAnim->setEndValue(endval);

    wAnim->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *bAnim = new QPropertyAnimation(ui->buttonWidget, "pos");
    bAnim->setStartValue(ui->buttonWidget->pos());
    QPoint bendval = ui->buttonWidget->pos();
    bendval.setY(230);
    bAnim->setEndValue(bendval);

    bAnim->setEasingCurve(QEasingCurve::OutQuad);

    QParallelAnimationGroup *group = new QParallelAnimationGroup();
    group->addAnimation(wAnim);
    group->addAnimation(bAnim);
    connect(group, &QParallelAnimationGroup::finished, [this] () {
         this->setMaximumHeight(267);
    });

    ui->statusPlainTextEdit->setHidden(true);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWidget::animShowStat()
{
    ui->statPushButton->setText(tr("隐藏"));
    QPropertyAnimation *wAnim = new QPropertyAnimation(this, "geometry");
    wAnim->setStartValue(this->geometry());
    this->setMaximumHeight(400);
    QRect endval = this->geometry();
    endval.setHeight(400);
    wAnim->setEndValue(endval);

    wAnim->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *bAnim = new QPropertyAnimation(ui->buttonWidget, "pos");
    bAnim->setStartValue(ui->buttonWidget->pos());
    QPoint bendval = ui->buttonWidget->pos();
    bendval.setY(360);
    bAnim->setEndValue(bendval);

    bAnim->setEasingCurve(QEasingCurve::OutQuad);

    QParallelAnimationGroup *group = new QParallelAnimationGroup();
    group->addAnimation(wAnim);
    group->addAnimation(bAnim);

    connect(group, &QParallelAnimationGroup::finished, [this] () {
        ui->statusPlainTextEdit->setHidden(false);
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWidget::timer_hide_status() {
    animHideStat();
}

void MainWidget::on_nameComboBox_editTextChanged(const QString &arg1)
{
    if (settings->contains(arg1 + "/password"))
    {
        settings->beginGroup(arg1);
        ui->pwdLineEdit->setText(settings->value("password").toString());
        int ifaceind = ui->ifaceComboBox->findText(settings->value("iface").toString());
        if (ifaceind != -1)
            ui->ifaceComboBox->setCurrentIndex(ifaceind);
        settings->endGroup();
    }
    else
        ui->pwdLineEdit->setText("");
}

void MainWidget::on_submitPushButton_clicked()
{
    freeze();

    if (!hasLogin) {
        QString name = ui->nameComboBox->currentText().trimmed();
        QString pwd = ui->pwdLineEdit->text().trimmed();

        QVariantMap userinfo;
        userinfo.insert("username", name);
        userinfo.insert("password", pwd);
        userinfo.insert("interface", ui->ifaceComboBox->currentText());
        eapdaemonif->Login(userinfo);
        ui->submitPushButton->setText(tr("正在登录"));

        ui->statusPlainTextEdit->clear();
        animShowStat();
    }
    else {
        eapdaemonif->Logoff();
        ui->submitPushButton->setText(tr("正在下线"));
        hasLogin = false;
    }
}

void MainWidget::freeze() {
    ui->nameComboBox->setEnabled(false);
    ui->pwdLineEdit->setEnabled(false);
    ui->ifaceComboBox->setEnabled(false);
    ui->submitPushButton->setEnabled(false);
}

void MainWidget::unfreeze() {
    ui->nameComboBox->setEnabled(true);
    ui->pwdLineEdit->setEnabled(true);
    ui->ifaceComboBox->setEnabled(true);
    ui->submitPushButton->setEnabled(true);
}
