#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QIntValidator>

class ipAddress : public QWidget
{
    Q_OBJECT
public:
    explicit ipAddress(QWidget *parent = 0);
    ~ipAddress();

    bool validate(void);
    bool subCheck(QLineEdit*);
    QString getIPAddressString(void);
    QHostAddress getQHostAddress(void);
    void textClear(void);
    QHBoxLayout *getHLayout(void);

signals:

public slots:

public:

    QLabel *labeladdr;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;

    QHBoxLayout *mainLayout;

    QLineEdit *number1;
    QLineEdit *number2;
    QLineEdit *number3;
    QLineEdit *number4;

    QIntValidator *validator;
};

#endif // IPADDRESS_H
