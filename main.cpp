#include "qlinkkeeper.h"
#include <QApplication>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLinkKeeper w;

    // Remove "?" Button from QDialog
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    w.setWindowFlags(flags);

    //qDebug("QDateTime::currentDateTime().toTime_t() is %lld", QDateTime::currentDateTime().toMSecsSinceEpoch());
    //qDebug("QDateTime::currentDateTime().toString() is %s", QDateTime::currentDateTime().toString().toLatin1().constData());

    w.show();

    return a.exec();
}
