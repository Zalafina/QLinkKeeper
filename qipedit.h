#ifndef QIPEDIT_H
#define QIPEDIT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QIntValidator>

class QIPEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QIPEdit(QWidget *parent = 0);
    ~QIPEdit();

    bool validate(void);
    bool subCheck(QLineEdit*);
    QString getIPAddressString(void);
    QHostAddress getQHostAddress(void);
    void textClear(void);
    QHBoxLayout *getHLayout(void);

signals:

public slots:

public:

    QLabel *labelip;
    QLabel *labelpoint1;
    QLabel *labelpoint2;
    QLabel *labelpoint3;

    QHBoxLayout *mainLayout;

    QLineEdit *number1;
    QLineEdit *number2;
    QLineEdit *number3;
    QLineEdit *number4;

    QIntValidator *validator;
};

#endif // QIPEDIT_H
