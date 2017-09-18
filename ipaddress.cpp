#include "ipaddress.h"

ipAddress::ipAddress(QWidget *parent) :
    QWidget(parent),
    validator(new QIntValidator(0,255))
{
    // Set Layout
    mainLayout=new QHBoxLayout(this);
    labeladdr=new QLabel("IP",this);
    label1=new QLabel(".",this);
    label2=new QLabel(".",this);
    label3=new QLabel(".",this);

    number1=new QLineEdit(this);
    number2=new QLineEdit(this);
    number3=new QLineEdit(this);
    number4=new QLineEdit(this);

    number1->setMaxLength(3);
    number2->setMaxLength(3);
    number3->setMaxLength(3);
    number4->setMaxLength(3);

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

    mainLayout->addWidget(labeladdr);
    mainLayout->addWidget(number1);
    mainLayout->addWidget(label1);
    mainLayout->addWidget(number2);
    mainLayout->addWidget(label2);
    mainLayout->addWidget(number3);
    mainLayout->addWidget(label3);
    mainLayout->addWidget(number4);

    parent->setLayout(mainLayout);
}

ipAddress::~ipAddress()
{
    if(validator != NULL){
        delete validator;
        validator = NULL;
    }
    else{
        // do nothing.
    }
}

bool ipAddress::validate(void)
{
    // Validate of the IP Address
    return subCheck(number1) && subCheck(number2) && subCheck(number3) && subCheck(number4);
}

bool ipAddress::subCheck(QLineEdit *number)
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

QString ipAddress::getIPAddressString(void)
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

QHostAddress ipAddress::getQHostAddress(void)
{
    QHostAddress hostAddr(getIPAddressString());

    return hostAddr;
}

void ipAddress::textClear(void)
{
    number1->clear();
    number2->clear();
    number3->clear();
    number4->clear();
}

QHBoxLayout *ipAddress::getHLayout(void)
{
    return mainLayout;
}
