#include "qlinkkeeper.h"
#include "ui_qlinkkeeper.h"

static const QString SUCCESS_ACTIVE(QString("<html><head/><body><p><span style=\" color:#00aa00;\">Success</span></p></body></html>"));
static const QString FAILURE_ACTIVE(QString("<html><head/><body><p><span style=\" color:#aa0000;\">Failure</span></p></body></html>"));

static const QString SUCCESS_INACTIVE(QString("<html><head/><body><p><span style=\" color:#737373;\">Success</span></p></body></html>"));
static const QString FAILURE_INACTIVE(QString("<html><head/><body><p><span style=\" color:#737373;\">Failure</span></p></body></html>"));

QLinkKeeper::QLinkKeeper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLinkKeeper),
    w_IPAddr(new QIPEdit(this)),
    m_LinkKeepStatus(LINK_IDLE),
    m_LinkKeeping(false),
    m_LinkTimer(this)
{
    ui->setupUi(this);

    QHBoxLayout *hlayout = w_IPAddr->getHLayout();
    hlayout->setAlignment(Qt::AlignTop);

    loadIPAddr();
    loadLinkCycleTime();

    QObject::connect(&m_LinkTimer, SIGNAL(timeout()), this, SLOT(linkCycleTimeOut()));
}

QLinkKeeper::~QLinkKeeper()
{
    delete ui;
}

void QLinkKeeper::on_linkButton_clicked()
{
    if (false == m_LinkKeeping){
        if (true == w_IPAddr->validate()){
            if (true == ui->clearCheckBox->isChecked()){
                ui->SuccessCounter->display(0);
                ui->FailureCounter->display(0);
            }

            w_IPAddr->setReadOnly(true);
            ui->cycleTimeSpinBox->setReadOnly(true);
            saveIPAddr();
            saveLinkCycleTime();
            ui->linkButton->setText("Stop");
            m_LinkTimer.start(ui->cycleTimeSpinBox->value()*1000);

            m_LinkKeeping = true;
        }
        else{
            QMessageBox::warning(this, tr("QLinkKeeper"), tr("无效的IP地址"));
        }
    }
    else{
        w_IPAddr->setReadOnly(false);
        ui->cycleTimeSpinBox->setReadOnly(false);
        ui->labelSuccess->setText(SUCCESS_INACTIVE);
        ui->labelFailure->setText(FAILURE_INACTIVE);
        ui->linkButton->setText("Start");

        m_LinkTimer.stop();
        m_LinkKeepStatus = LINK_IDLE;
        m_LinkKeeping = false;
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

void QLinkKeeper::linkCycleTimeOut(void)
{
    QHostAddress ipaddr = w_IPAddr->getQHostAddress();
    QProcess *cmd = new QProcess;
    QString strArg = "ping " + ipaddr.toString() + " -n 1 -w " + QString::number(8000);

    cmd->start(strArg);
    cmd->waitForReadyRead();
    cmd->waitForFinished();
    QString retStr = cmd->readAll();

//#ifdef DEBUG_LOGOUT_ON
//    qDebug() <<"Ping response: " << retStr;
//#endif

    if (true == retStr.contains( QString("TTL=") ))
    {
        ui->labelSuccess->setText(SUCCESS_ACTIVE);
        int successCount = ui->SuccessCounter->intValue();
        ui->SuccessCounter->display(successCount+1);

#ifdef DEBUG_LOGOUT_ON
        qDebug("Server %s connect Success(%d)", ipaddr.toString().toLatin1().constData(), ui->SuccessCounter->intValue());
#endif
    }
    else{
        ui->labelFailure->setText(FAILURE_ACTIVE);
        int failureCount = ui->FailureCounter->intValue();
        ui->FailureCounter->display(failureCount+1);

#ifdef DEBUG_LOGOUT_ON
        qDebug("Server %s connect Failure(%d)", ipaddr.toString().toLatin1().constData(), ui->FailureCounter->intValue());
#endif
    }
    retStr.clear();
}

void QLinkKeeper::saveIPAddr(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);

    QHostAddress ipaddr = w_IPAddr->getQHostAddress();
    quint32 ipv4addr = ipaddr.toIPv4Address();
    settingFile.setValue(QLatin1String("IPv4Address"), ipv4addr);

#ifdef DEBUG_LOGOUT_ON
    qDebug("Save IP Address (%s)", ipaddr.toString().toLatin1().constData());
#endif
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

        validateResult = QIPEdit::validateIPAddrString(ipAddrString);

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

void QLinkKeeper::saveLinkCycleTime(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);

    int cycletime = ui->cycleTimeSpinBox->value();
    settingFile.setValue(QLatin1String("LinkCycleTime"), cycletime);

#ifdef DEBUG_LOGOUT_ON
    qDebug("Save LinkCycleTime (%d) Seconds", cycletime);
#endif
}

void QLinkKeeper::loadLinkCycleTime(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);
    bool loadResult = false;
    bool containsFlag = settingFile.contains("LinkCycleTime");
    int cycletime = 0;

    if (true == containsFlag){
        cycletime = settingFile.value("LinkCycleTime").toInt();

        if (1 <= cycletime && cycletime <= 9000){
            ui->cycleTimeSpinBox->setValue(cycletime);
            loadResult = true;
        }
    }

#ifdef DEBUG_LOGOUT_ON
    if (true == loadResult){
        qDebug("Load LinkCycleTime (%d) Seconds", cycletime);
    }
    else{
        qDebug("Load LinkCycleTime Failure: containsFlag(%d), cycletime(%d)", containsFlag, cycletime);
    }
#endif
}
