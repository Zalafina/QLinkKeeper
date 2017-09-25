#include "qlinkkeeper.h"
#include "ui_qlinkkeeper.h"

static const QString SUCCESS_ACTIVE(QString("<html><head/><body><p><span style=\" color:#00aa00;\">Success</span></p></body></html>"));
static const QString FAILURE_ACTIVE(QString("<html><head/><body><p><span style=\" color:#aa0000;\">Failure</span></p></body></html>"));

QLinkKeeper::QLinkKeeper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLinkKeeper),
    w_IPAddr(new QIPEdit(this))
{
    ui->setupUi(this);
    QHBoxLayout *hlayout = w_IPAddr->getHLayout();
    hlayout->setAlignment(Qt::AlignTop);
}

QLinkKeeper::~QLinkKeeper()
{
    delete ui;
}

void QLinkKeeper::on_startButton_clicked()
{
    ui->labelSuccess->setText(SUCCESS_ACTIVE);

    //    QMetaEnum my_enum = QMetaEnum::fromType<LinkKeepStatus>();
    //    qDebug("QLinkKeeper::LINK_SUCCESS(%s)", my_enum.valueToKey(QLinkKeeper::LINK_SUCCESS));
    //    for (int loop=0; loop<my_enum.keyCount(); ++loop)
    //    {
    //        qDebug("%s=%d", my_enum.key(loop), my_enum.value(loop));
    //    }

    //    QHostAddress ipaddr("192.168.210.56");
    //    quint32 addr_value = ipaddr.toIPv4Address();
    //    qDebug("ipaddr.toIPv4Address() = %u", addr_value);
}

void QLinkKeeper::on_saveipButton_clicked()
{
    ui->labelFailure->setText(FAILURE_ACTIVE);
}
