#include "qipedit.h"

QIPEdit::QIPEdit(QWidget *parent) :
    QWidget(parent),
    validator(new QIntValidator(0,255))
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

void QIPEdit::textClear(void)
{
    number1->clear();
    number2->clear();
    number3->clear();
    number4->clear();
}

QHBoxLayout *QIPEdit::getHLayout(void)
{
    return mainLayout;
}
