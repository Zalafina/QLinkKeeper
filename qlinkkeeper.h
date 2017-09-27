#ifndef QLINKKEEPER_H
#define QLINKKEEPER_H

#include <QDialog>
#include <QToolButton>
#include <QMetaEnum>
#include <QTimer>
#include <QHostAddress>
#include <QMessageBox>
#include <QSettings>
#include <QTcpSocket>
#include <QProcess>
#include "qipedit.h"

namespace Ui {
class QLinkKeeper;
}

class QLinkKeeper : public QDialog
{
    Q_OBJECT

public:
    explicit QLinkKeeper(QWidget *parent = 0);
    ~QLinkKeeper();

    enum LinkKeepStatus
    {
        LINK_IDLE = 0U,
        LINK_SUCCESS,
        LINK_FAILURE
    };
    Q_ENUM(LinkKeepStatus)

private slots:
    void on_linkButton_clicked();

    void linkCycleTimeOut(void);
    void readPingOutputData(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void saveIPAddr(void);
    void loadIPAddr(void);

    void saveLinkCycleTime(void);
    void loadLinkCycleTime(void);

private:
    Ui::QLinkKeeper *ui;
    QIPEdit *w_IPAddr;
    quint8 m_LinkKeepStatus;
    bool m_LinkKeeping;
    QTimer m_LinkTimer;
    QProcess *m_PingProcess;
};

#endif // QLINKKEEPER_H
