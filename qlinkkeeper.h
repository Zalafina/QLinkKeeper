#ifndef QLINKKEEPER_H
#define QLINKKEEPER_H

#include <QDialog>
#include <QMetaEnum>
#include <QHostAddress>
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
    void on_startButton_clicked();

    void on_saveipButton_clicked();

private:
    Ui::QLinkKeeper *ui;
    QIPEdit *w_IPAddr;
};

#endif // QLINKKEEPER_H
