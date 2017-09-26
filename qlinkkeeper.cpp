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

    loadIPAddr();
}

QLinkKeeper::~QLinkKeeper()
{
    delete ui;
}

void QLinkKeeper::on_startButton_clicked()
{
    if (true == w_IPAddr->validate()){
        saveIPAddr();

        ui->labelSuccess->setText(SUCCESS_ACTIVE);
    }
    else{
        QMessageBox::warning(this, tr("QLinkKeeper"), tr("无效的IP地址"));
    }

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

void QLinkKeeper::on_clearCountButton_clicked()
{
    ui->labelFailure->setText(FAILURE_ACTIVE);
}

void QLinkKeeper::saveIPAddr(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);

    QHostAddress ipaddr = w_IPAddr->getQHostAddress();
    quint32 ipv4addr = ipaddr.toIPv4Address();
#ifdef DEBUG_LOGOUT_ON
    qDebug("Save IP Address (%s)", ipaddr.toString().toLatin1().constData());
#endif

    settingFile.setValue(QLatin1String("IPv4Address"), ipv4addr);
}

void QLinkKeeper::loadIPAddr(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);
    bool validateResult = false;
    bool loadResult = false;
    bool containsFlag = settingFile.contains("IPv4Address");
    QHostAddress ipaddr;

    if (true == containsFlag){
        quint32 ipv4addr = settingFile.value("IPv4Address").toInt();
        ipaddr.setAddress(ipv4addr);

        QString ipAddrString = ipaddr.toString();

        bool validateResult = QIPEdit::validateIPAddrString(ipAddrString);

        if (true == validateResult){
            loadResult = w_IPAddr->setIPAddressFromString(ipAddrString);
        }
    }

#ifdef DEBUG_LOGOUT_ON
    if (true == loadResult){
        qDebug("Load IP Address (%s)", ipaddr.toString().toLatin1().constData());
    }
    else{
        qDebug("Load IP Address Failure: containsFlag(%d)", containsFlag);
    }
#endif
}
