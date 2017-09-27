#include "qlinkkeeper.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    //qDebug() << QStyleFactory::keys();
    QLinkKeeper w;

    // Remove "?" Button from QDialog
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowMinimizeButtonHint;
    w.setWindowFlags(flags);

    //qDebug("QDateTime::currentDateTime().toTime_t() is %lld", QDateTime::currentDateTime().toMSecsSinceEpoch());
    //qDebug("QDateTime::currentDateTime().toString() is %s", QDateTime::currentDateTime().toString().toLatin1().constData());

    w.show();

    return a.exec();
}
