#include "qlinkkeeper.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QLinkKeeper w;

    // Remove "?" Button from QDialog
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowMinimizeButtonHint;
    w.setWindowFlags(flags);

    w.show();

    return a.exec();
}
