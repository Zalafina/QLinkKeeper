#include "qipedit.h"

static const int IP_NUM_MIN = 0;
static const int IP_NUM_MAX = 255;

QIPEdit::QIPEdit(QWidget *parent) :
    QWidget(parent),
    validator(new QIntValidator(IP_NUM_MIN,IP_NUM_MAX))
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

    number1->setInputMethodHints(Qt::ImhDigitsOnly);
    number2->setInputMethodHints(Qt::ImhDigitsOnly);
    number3->setInputMethodHints(Qt::ImhDigitsOnly);
    number4->setInputMethodHints(Qt::ImhDigitsOnly);

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

    number1->installEventFilter(this);
    number2->installEventFilter(this);
    number3->installEventFilter(this);
    number4->installEventFilter(this);
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
            if ((IP_NUM_MIN <= num1 && num1 <= IP_NUM_MAX)
                && (IP_NUM_MIN <= num2 && num2 <= IP_NUM_MAX)
                && (IP_NUM_MIN <= num3 && num3 <= IP_NUM_MAX)
                && (IP_NUM_MIN <= num4 && num4 <= IP_NUM_MAX)){
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

bool QIPEdit::eventFilter(QObject *widgetobject, QEvent *event)
{
    if ((widgetobject == number1)
            || (widgetobject == number2)
            || (widgetobject == number3)
            || (widgetobject == number4)){
        if (event->type() == QEvent::KeyPress){
            int widgetindex_cur = 0;
            int widgetindex_next = 0;
            bool shiftmodifier  = false;
            QKeyEvent *keyevent = dynamic_cast<QKeyEvent *>(event);

            if (widgetobject == number1){
                widgetindex_cur = 1;
            }
            else if (widgetobject == number2){
                widgetindex_cur = 2;
            }
            else if (widgetobject == number3){
                widgetindex_cur = 3;
            }
            else {
                widgetindex_cur = 4;
            }

            switch (keyevent->key()) {
            case Qt::Key_Space:
                if (keyevent->modifiers() == (Qt::ShiftModifier)){
                    if (1 == widgetindex_cur){
                        widgetindex_next = 4;
                    }
                    else{
                        widgetindex_next = widgetindex_cur - 1;
                    }
                    shiftmodifier = true;
                }
                else{
                    if (4 == widgetindex_cur){
                        widgetindex_next = 1;
                    }
                    else{
                        widgetindex_next = widgetindex_cur + 1;
                    }
                }

                if (4 == widgetindex_next){
                    number4->setFocus(Qt::TabFocusReason);
                }
                else if (3 == widgetindex_next){
                    number3->setFocus(Qt::TabFocusReason);
                }
                else if (2 == widgetindex_next){
                    number2->setFocus(Qt::TabFocusReason);
                }
                else {
                    number1->setFocus(Qt::TabFocusReason);
                }
                break;
            default:
                break;
            }

#ifdef DEBUG_LOGOUT_ON
            if (widgetindex_next != 0){
                qDebug().nospace() << "[QIPEdit]" << " KeyPressed " << (Qt::Key)keyevent->key() << " with Qt::ShiftModifier("<<shiftmodifier<<") " << "on IP Number " << widgetindex_cur << ", Focus change to IP Number " << widgetindex_next;
            }
#endif
        }
    }

    return QWidget::eventFilter(widgetobject, event);
}
