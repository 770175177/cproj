#-------------------------------------------------
#
# Project created by QtCreator 2018-11-12T16:56:13
#
#-------------------------------------------------

QT       += core gui network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mmmm
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    keting_dialog.cpp \
    woshi_dialog.cpp \
    caidan_dialog.cpp \
    chufang_dialog.cpp \
    camera_dialog.cpp \
    control_dialog.cpp

HEADERS += \
        widget.h \
    keting_dialog.h \
    woshi_dialog.h \
    caidan_dialog.h \
    chufang_dialog.h \
    camera_dialog.h \
    control_dialog.h

FORMS += \
        widget.ui \
    keting_dialog.ui \
    woshi_dialog.ui \
    caidan_dialog.ui \
    chufang_dialog.ui \
    camera_dialog.ui \
    control_dialog.ui

RESOURCES += \
    xxx.qrc
