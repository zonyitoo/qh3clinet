#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSettings>
#include <memory>
#include "eapdaemoninterface.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void closeEvent(QCloseEvent *);
    
private:
    void animShowStat();
    void animHideStat();

private slots:
    void on_statPushButton_clicked();

    void on_nameComboBox_editTextChanged(const QString &arg1);

    void on_submitPushButton_clicked();

    void timer_hide_status();

private:
    void freeze();
    void unfreeze();

    Ui::MainWidget *ui;

    QSettings *settings;

    std::auto_ptr<EAPDaemonInterface> eapdaemonif;
    bool hasLogin;
};

#endif // MAINWIDGET_H
