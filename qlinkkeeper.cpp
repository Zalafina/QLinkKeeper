#include "qlinkkeeper.h"
#include "ui_qlinkkeeper.h"

QLinkKeeper::QLinkKeeper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLinkKeeper),
    w_IPAddr(new ipAddress(this))
{
    ui->setupUi(this);
    QHBoxLayout *hlayout = w_IPAddr->getHLayout();
    hlayout->setAlignment(Qt::AlignTop);

    QHostAddress ipaddr("192.168.210.56");
    quint32 addr_value = ipaddr.toIPv4Address();
    qDebug("ipaddr.toIPv4Address() = %u", addr_value);
}

QLinkKeeper::~QLinkKeeper()
{
    delete ui;
}
