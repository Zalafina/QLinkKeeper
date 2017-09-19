#-------------------------------------------------
#
# Project created by QtCreator 2017-09-18T08:52:14
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLinkKeeper
TEMPLATE = app

CONFIG(debug, debug|release){
    DEFINES += DEBUG_LOGOUT_ON
    message("Debug Build")
}

CONFIG(release, debug|release){
    message("Release Build")
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    qlinkkeeper.cpp \
    qipedit.cpp

HEADERS  += qlinkkeeper.h \
    qipedit.h

FORMS    += qlinkkeeper.ui
