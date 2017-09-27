#include "qipedit.h"

QIPEdit::QIPEdit(QWidget *parent) :
    QWidget(parent),
    validator(new QIntValidator(1,254))
{
    // Set Layout
    mainLayout=new QHBoxLayout(this);
    labelip=new QLabel("IP",this);
    labelpoint1=new QLabel(".",this);
    labelpoint2=new QLabel(".",this);
    labelpoint3=new QLabel(".",this);

    number1=new QLineEdit(this);
    number2=new QLineEdit(this);
    number3=new QLineEdit(this);
    number4=new QLineEdit(this);

    number1->setMaxLength(3);
    number2->setMaxLength(3);
    number3->setMaxLength(3);
    number4->setMaxLength(3);

    labelpoint1->setMaximumWidth(4);
    labelpoint2->setMaximumWidth(4);
    labelpoint3->setMaximumWidth(4);

    // QRegExp ip_number("[0-9]*");
    // number1->setValidator(new QRegExpValidator(ip_number, number1));
    number1->setValidator(validator);
    number2->setValidator(validator);
    number3->setValidator(validator);
    number4->setValidator(validator);

    number1->setAttribute(Qt::WA_InputMethodEnabled, false);
    number2->setAttribute(Qt::WA_InputMethodEnabled, false);
    number3->setAttribute(Qt::WA_InputMethodEnabled, false);
    number4->setAttribute(Qt::WA_InputMethodEnabled, false);

    Qt::Alignment alignment = Qt::AlignHCenter|Qt::AlignVCenter;
    number1->setAlignment(alignment);
    number2->setAlignment(alignment);
    number3->setAlignment(alignment);
    number4->setAlignment(alignment);

    labelpoint1->setAlignment(alignment);
    labelpoint2->setAlignment(alignment);
    labelpoint3->setAlignment(alignment);

    mainLayout->addWidget(labelip);
    mainLayout->addWidget(number1);
    mainLayout->addWidget(labelpoint1);
    mainLayout->addWidget(number2);
    mainLayout->addWidget(labelpoint2);
    mainLayout->addWidget(number3);
    mainLayout->addWidget(labelpoint3);
    mainLayout->addWidget(number4);

    parent->setLayout(mainLayout);
}

QIPEdit::~QIPEdit()
{
    if(validator != NULL){
        delete validator;
        validator = NULL;
    }
    else{
        // do nothing.
    }
}

bool QIPEdit::validate(void)
{
    // Validate of the IP Address
    return subCheck(number1) && subCheck(number2) && subCheck(number3) && subCheck(number4);
}

bool QIPEdit::subCheck(QLineEdit *number)
{
    QString str = number->displayText();
    bool checkresult = true;

    if (str.length() > 0){
        checkresult = true;
    }
    else{
        checkresult = false;
    }

    return checkresult;
}

QString QIPEdit::getIPAddressString(void)
{
    QString str;

    if (true == validate()){
        str+=number1->text();
        str+=".";
        str+=number2->text();
        str+=".";
        str+=number3->text();
        str+=".";
        str+=number4->text();
    }

    return str;
}

QHostAddress QIPEdit::getQHostAddress(void)
{
    QHostAddress hostAddr(getIPAddressString());

    return hostAddr;
}

bool QIPEdit::setIPAddressFromString(QString &IPString)
{
    bool setResult = false;
    if (true == validateIPAddrString(IPString)){
        QString str1 = IPString.section('.', 0, 0);
        QString str2 = IPString.section('.', 1, 1);
        QString str3 = IPString.section('.', 2, 2);
        QString str4 = IPString.section('.', 3, 3);

        number1->setText(str1);
        number2->setText(str2);
        number3->setText(str3);
        number4->setText(str4);

        setResult = true;
    }

    return setResult;
}

bool QIPEdit::validateIPAddrString(QString &IPString)
{
    bool validateResult = false;
    int pointCount = 0;
    bool result1 = false;
    bool result2 = false;
    bool result3 = false;
    bool result4 = false;
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    int num4 = 0;

    pointCount = IPString.count(QChar('.'));
    if (3 == pointCount){
        QString str1 = IPString.section('.', 0, 0);
        QString str2 = IPString.section('.', 1, 1);
        QString str3 = IPString.section('.', 2, 2);
        QString str4 = IPString.section('.', 3, 3);

        num1 = str1.toInt(&result1);
        num2 = str2.toInt(&result2);
        num3 = str3.toInt(&result3);
        num4 = str4.toInt(&result4);

        if (true == result1 && true == result2 && true == result3 && true == result4){
            if ((1 <= num1 && num1 <= 254)
                && (1 <= num2 && num2 <= 254)
                && (1 <= num3 && num3 <= 254)
                && (1 <= num4 && num4 <= 254)){
                validateResult = true;
            }
        }
    }

    return validateResult;
}

void QIPEdit::textClear(void)
{
    number1->clear();
    number2->clear();
    number3->clear();
    number4->clear();
}

void QIPEdit::setReadOnly(bool r)
{
    number1->setReadOnly(r);
    number2->setReadOnly(r);
    number3->setReadOnly(r);
    number4->setReadOnly(r);
}

QHBoxLayout *QIPEdit::getHLayout(void)
{
    return mainLayout;
}
