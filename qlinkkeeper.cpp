#include <QTextStream>
#include "qlinkkeeper.h"
#include "ui_qlinkkeeper.h"

static const QString SUCCESS_ACTIVE(QString("<html><head/><body><p><span style=\" color:#00aa00;\">Success</span></p></body></html>"));
static const QString FAILURE_ACTIVE(QString("<html><head/><body><p><span style=\" color:#aa0000;\">Failure</span></p></body></html>"));

static const QString SUCCESS_INACTIVE(QString("<html><head/><body><p><span style=\" color:#737373;\">Success</span></p></body></html>"));
static const QString FAILURE_INACTIVE(QString("<html><head/><body><p><span style=\" color:#737373;\">Failure</span></p></body></html>"));

static const uint PING_CMD_TIMEOUT = 10000U;
static const int LCDNUMBER_MAX = 999999;

QLinkKeeper::QLinkKeeper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLinkKeeper),
    w_IPAddr(new QIPEdit(this)),
    m_LinkKeepStatus(LINK_IDLE),
    m_LinkKeeping(false),
    m_LinkTimer(this),
    m_PingProcess(NULL),
    m_SysTrayIcon(NULL)
{
    ui->setupUi(this);
    //setFocusPolicy(Qt::StrongFocus);

    ui->cycleTimeSpinBox->setInputMethodHints(Qt::ImhDigitsOnly);
    ui->cycleTimeSpinBox->setAttribute(Qt::WA_InputMethodEnabled, false);

//    w_IPAddr->number1->installEventFilter(this);
//    w_IPAddr->number2->installEventFilter(this);
//    w_IPAddr->number3->installEventFilter(this);
//    w_IPAddr->number4->installEventFilter(this);

    QHBoxLayout *hlayout = w_IPAddr->getHLayout();
    hlayout->setAlignment(Qt::AlignTop);

    loadIPAddr();
    loadLinkCycleTime();
    loadClearCountStatus();

    QObject::connect(&m_LinkTimer, SIGNAL(timeout()), this, SLOT(linkCycleTimeOut()));

    m_PingProcess = new QProcess();
    m_PingProcess->setProcessChannelMode(QProcess::MergedChannels);
    //static_cast<void>(QObject::connect(m_PingProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(receivePingStandOutputData())));
    QObject::connect(m_PingProcess,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(readPingOutputData(int, QProcess::ExitStatus)));

    m_SysTrayIcon = new QSystemTrayIcon(this);
    m_SysTrayIcon->setIcon(QIcon(":/AppIcon.ico"));
    m_SysTrayIcon->setToolTip("QLinkKeeper(Idle)");
    m_SysTrayIcon->show();

    QObject::connect(m_SysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(SystrayIconActivated(QSystemTrayIcon::ActivationReason)));
}

QLinkKeeper::~QLinkKeeper()
{
    delete ui;

    delete w_IPAddr;
    w_IPAddr = NULL;

    m_PingProcess->terminate();
    delete m_PingProcess;
    m_PingProcess = NULL;

    delete m_SysTrayIcon;
    m_SysTrayIcon = NULL;
}

void QLinkKeeper::WindowStateChangedProc(void)
{
    if (true == isMinimized()){
#ifdef DEBUG_LOGOUT_ON
        qDebug("QLinkKeeper::WindowStateChangedProc() -> Window Minimized: setHidden!");
#endif
        hide();
    }
}

void QLinkKeeper::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::WindowStateChange)
    {
        QTimer::singleShot(0, this, SLOT(WindowStateChangedProc()));
    }
    QDialog::changeEvent(event);
}

//void QLinkKeeper::keyPressEvent(QKeyEvent *event)
//{
//    switch (event->key()) {
//    case Qt::Key_Space:
//        focusNextChild();
//        break;
//    default:
//        break;
//    }

//    qDebug() << "Some key pressed:" << Qt::Key(event->key());

//    QDialog::keyPressEvent(event);
//}

//bool QLinkKeeper::eventFilter(QObject *widgetobject, QEvent *event)
//{
//    if ((widgetobject == w_IPAddr->number1)
//            || (widgetobject == w_IPAddr->number2)
//            || (widgetobject == w_IPAddr->number3)
//            || (widgetobject == w_IPAddr->number4)){
//        if (event->type() == QEvent::KeyPress){
//            int widgetindex_cur = 0;
//            int widgetindex_next = 0;
//            bool shiftmodifier  = false;
//            QKeyEvent *keyevent = dynamic_cast<QKeyEvent *>(event);

//            if (widgetobject == w_IPAddr->number1){
//                widgetindex_cur = 1;
//            }
//            else if (widgetobject == w_IPAddr->number2){
//                widgetindex_cur = 2;
//            }
//            else if (widgetobject == w_IPAddr->number3){
//                widgetindex_cur = 3;
//            }
//            else {
//                widgetindex_cur = 4;
//            }

//            switch (keyevent->key()) {
//            case Qt::Key_Space:
//                if (keyevent->modifiers() == (Qt::ShiftModifier)){
//                    if (1 == widgetindex_cur){
//                        widgetindex_next = 4;
//                    }
//                    else{
//                        widgetindex_next = widgetindex_cur - 1;
//                    }
//                    shiftmodifier = true;
//                }
//                else{
//                    if (4 == widgetindex_cur){
//                        widgetindex_next = 1;
//                    }
//                    else{
//                        widgetindex_next = widgetindex_cur + 1;
//                    }
//                }

//                if (4 == widgetindex_next){
//                    w_IPAddr->number4->setFocus(Qt::TabFocusReason);
//                }
//                else if (3 == widgetindex_next){
//                    w_IPAddr->number3->setFocus(Qt::TabFocusReason);
//                }
//                else if (2 == widgetindex_next){
//                    w_IPAddr->number2->setFocus(Qt::TabFocusReason);
//                }
//                else {
//                    w_IPAddr->number1->setFocus(Qt::TabFocusReason);
//                }
//                break;
//            default:
//                break;
//            }

//            qDebug() << "KeyPressed->" << (Qt::Key)keyevent->key() << "with Qt::ShiftModifier("<<shiftmodifier<<")" << "on IP Number" << widgetindex_cur;
//        }
//    }

//    return QDialog::eventFilter(widgetobject, event);
//}

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
            saveClearCountStatus();
            ui->linkButton->setText("Stop");
            m_LinkTimer.stop();
            m_LinkTimer.start(ui->cycleTimeSpinBox->value()*1000);
            //linkCycleTimeOut();

            m_LinkKeeping = true;
            hide();
        }
        else{
            QMessageBox::warning(this, tr("QLinkKeeper"), tr("无效的IP地址"));
        }
    }
    else{
        m_PingProcess->terminate();
        w_IPAddr->setReadOnly(false);
        ui->cycleTimeSpinBox->setReadOnly(false);
        ui->labelSuccess->setText(SUCCESS_INACTIVE);
        ui->labelFailure->setText(FAILURE_INACTIVE);
        ui->linkButton->setText("Start");

        m_LinkTimer.stop();
        m_LinkKeepStatus = LINK_IDLE;
        m_SysTrayIcon->setToolTip("QLinkKeeper(Idle)");
        m_LinkKeeping = false;
    }
}

void QLinkKeeper::linkCycleTimeOut(void)
{
    QHostAddress ipaddr = w_IPAddr->getQHostAddress();
    QString ping_cmd = "ping " + ipaddr.toString() + " -n 1 -w " + QString::number(PING_CMD_TIMEOUT);

    QProcess::ProcessState process_state = m_PingProcess->state();

    if (process_state == QProcess::NotRunning){
        m_PingProcess->start(ping_cmd, QIODevice::ReadOnly);
    }
    else{
        // do nothing.
    }
}

void QLinkKeeper::readPingOutputData(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    bool pingresult = false;
    QString ipaddr_str = w_IPAddr->getIPAddressString();

    while (!m_PingProcess->atEnd()) {
        QByteArray data = m_PingProcess->readAllStandardOutput();

#ifdef DEBUG_LOGOUT_ON
        //QTextCodec *pingCodeC = QTextCodec::codecForName("GB2312");
        QTextCodec *pingCodeC = QTextCodec::codecForLocale();
        QString codecResult = pingCodeC->toUnicode(data);
        QStringList pingoutput = codecResult.split("\r\n");

        //        QTextStream cout(stdout, QIODevice::WriteOnly);
        //        for(qint32 loopCnt = 0; loopCnt < pingoutput.size(); loopCnt++){
        //            cout << pingoutput.at(loopCnt) << endl; cout.flush();
        //        }

        for(qint32 loopCnt = 0; loopCnt < pingoutput.size(); loopCnt++){
            qDebug().noquote() << pingoutput.at(loopCnt);
        }
#endif

        if (true == data.contains("TTL="))
        {
            pingresult = true;
        }
        else{
            // do nothing.
        }
    }

    if (true == pingresult)
    {
        ui->labelSuccess->setText(SUCCESS_ACTIVE);

        int successCount = ui->SuccessCounter->intValue() + 1;
        if ( successCount > LCDNUMBER_MAX ){
            successCount = LCDNUMBER_MAX;
        }
        ui->SuccessCounter->display(successCount);

        m_LinkKeepStatus = LINK_SUCCESS;
        m_SysTrayIcon->setToolTip("QLinkKeeper(Success:" + QString::number(successCount) + ")");

#ifdef DEBUG_LOGOUT_ON
        qDebug("Server %s connect Success(%d)", ipaddr_str.toLatin1().constData(), ui->SuccessCounter->intValue());
#endif
    }
    else{
        ui->labelFailure->setText(FAILURE_ACTIVE);

        int failureCount = ui->FailureCounter->intValue() + 1;
        if ( failureCount > LCDNUMBER_MAX ){
            failureCount = LCDNUMBER_MAX;
        }
        ui->FailureCounter->display(failureCount);

        m_LinkKeepStatus = LINK_FAILURE;
        m_SysTrayIcon->setToolTip("QLinkKeeper(Failure:" + QString::number(failureCount) + ")");

        if (true == isHidden()){
            QString titlec("QLinkKeeper");
            QString textc = "Link Failure(" + QString::number(failureCount) + ")";
            m_SysTrayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information);
        }

#ifdef DEBUG_LOGOUT_ON
        qDebug("Server %s connect Failure(%d)", ipaddr_str.toLatin1().constData(), ui->FailureCounter->intValue());
#endif
    }
}

void QLinkKeeper::SystrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::DoubleClick == reason){
#ifdef DEBUG_LOGOUT_ON
        qDebug() << "QLinkKeeper::SystrayIconActivated() -> SystemTray double clicked: showNormal()!!";
#endif

        showNormal();
        activateWindow();
        raise();
        QApplication::setActiveWindow(this);
    }
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
        qDebug("Load IP Address Failure: containsFlag(%s)", QVariant::fromValue(containsFlag).toString().toLatin1().constData());
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
    int cycletime = -1;

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
        qDebug("Load LinkCycleTime Failure: containsFlag(%s), cycletime(%d)", QVariant::fromValue(containsFlag).toString().toLatin1().constData(), cycletime);
    }
#endif
}

void QLinkKeeper::saveClearCountStatus(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);

    bool clearcountStatus = ui->clearCheckBox->isChecked();
    settingFile.setValue(QLatin1String("ClearCount"), clearcountStatus);

#ifdef DEBUG_LOGOUT_ON
    QVariant ClearCount = ui->clearCheckBox->isChecked();
    qDebug("Save ClearCount (%s)", ClearCount.toString().toLatin1().constData());
#endif
}

void QLinkKeeper::loadClearCountStatus(void)
{
    QSettings settingFile(QString("settings.ini"), QSettings::IniFormat);
    bool loadResult = false;
    bool containsFlag = settingFile.contains("ClearCount");
    bool clearcountStatus = true;
    QVariant ClearCount;

    if (true == containsFlag){
        ClearCount = settingFile.value("ClearCount");
        if (true == ClearCount.canConvert<bool>() ){
            clearcountStatus = settingFile.value("ClearCount").toBool();
            ui->clearCheckBox->setChecked(clearcountStatus);
            loadResult = true;
        }
        else{
            // do nothing.
        }
    }
    else{
        // do nothing.
    }

#ifdef DEBUG_LOGOUT_ON
    if (true == loadResult){
        qDebug("Load ClearCount (%s)", ClearCount.toString().toLatin1().constData());
    }
    else{
        qDebug("Load ClearCount Failure: containsFlag(%s), canConvert(%s)", QVariant::fromValue(containsFlag).toString().toLatin1().constData(), QVariant::fromValue(ClearCount.canConvert<bool>()).toString().toLatin1().constData());
    }
#endif
}
