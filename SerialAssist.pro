#-------------------------------------------------
#
# Project created by QtCreator 2013-12-17T15:23:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(./src/qextserialport/qextserialport.pri)

TARGET = SerialAssist
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mycom.cpp

HEADERS  += mainwindow.h \
    mycom.h

FORMS    += mainwindow.ui

DEFINES += \
    LINUX \
