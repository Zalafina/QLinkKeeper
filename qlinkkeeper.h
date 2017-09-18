#ifndef QLINKKEEPER_H
#define QLINKKEEPER_H

#include <QDialog>
#include <QHostAddress>
#include "ipaddress.h"

namespace Ui {
class QLinkKeeper;
}

class QLinkKeeper : public QDialog
{
    Q_OBJECT

public:
    explicit QLinkKeeper(QWidget *parent = 0);
    ~QLinkKeeper();

private:
    Ui::QLinkKeeper *ui;
    ipAddress *w_IPAddr;
};

#endif // QLINKKEEPER_H
